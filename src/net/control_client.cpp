#include "control_client.h"

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QElapsedTimer>
#include <QCoreApplication>

#include <sstream>
#include <mutex>

namespace {
constexpr quint16 kCtrlPort = 46000;
constexpr int kRxTimeoutMs = 200;

QUdpSocket* socket_instance = nullptr;
std::mutex sock_mutex;
std::set<std::string> known_ids;
std::string self_id;
std::map<std::string, uint16_t> id_ports;
std::map<std::string, std::string> id_endpoints;
uint16_t self_port = 0;

using SockLock = std::unique_lock<std::mutex>;

static SockLock acquire_lock() {
    return SockLock(sock_mutex);
}

void ensure_socket() {
    if (socket_instance) return;
    socket_instance = new QUdpSocket(qApp);
    socket_instance->bind(QHostAddress::AnyIPv4, kCtrlPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void send_datagram(const QByteArray& data) {
    ensure_socket();
    socket_instance->writeDatagram(data, QHostAddress::Broadcast, kCtrlPort);
    socket_instance->writeDatagram(data, QHostAddress::LocalHost, kCtrlPort);
}

void pump_rx() {
    ensure_socket();
    while (socket_instance->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(socket_instance->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort = 0;
        socket_instance->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (datagram.startsWith("HELLO:")) {
            QList<QByteArray> parts = datagram.mid(6).split(':');
            std::string id = parts.value(0).toStdString();
            uint16_t port = 0;
            if (parts.size() > 1) {
                port = static_cast<uint16_t>(parts.value(1).toUShort());
            }
            std::string senderIp = sender.toString().toStdString();
            if (!self_id.empty() && id == self_id) {
                continue; // ignore our own echo
            }
            known_ids.insert(id);
            id_ports[id] = port;
            id_endpoints[id] = senderIp + ":" + std::to_string(port);
        } else if (datagram.startsWith("WHO") && !self_id.empty()) {
            QByteArray reply("HELLO:");
            reply.append(QString::fromStdString(self_id).toUtf8());
            reply.append(':');
            reply.append(QString::number(self_port).toUtf8());
            send_datagram(reply);
        }
    }
}

std::string list_join(const std::set<std::string>& ids) {
    std::string out;
    for (auto it = ids.begin(); it != ids.end(); ++it) {
        if (it != ids.begin()) out.push_back(',');
        out += *it;
    }
    return out;
}

bool starts_with(const std::string& s, const std::string& prefix) {
    return s.compare(0, prefix.size(), prefix) == 0;
}
} // namespace

static bool ensure_unique_name_locked(const std::string& client_id, uint16_t audio_port) {
    ensure_socket();
    self_id = client_id;
    self_port = audio_port;
    known_ids.clear();

    QByteArray hello("HELLO:");
    hello.append(QString::fromStdString(client_id).toUtf8());
    hello.append(':');
    hello.append(QString::number(audio_port).toUtf8());
    send_datagram(hello);

    QByteArray who("WHO");
    send_datagram(who);

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < kRxTimeoutMs) {
        if (socket_instance->waitForReadyRead(20)) {
            pump_rx();
        }
    }

    bool conflict = known_ids.find(client_id) != known_ids.end();
    known_ids.insert(client_id);
    id_ports[client_id] = audio_port;
    return !conflict;
}

bool ensure_unique_name(const std::string& client_id, uint16_t audio_port) {
    auto lock = acquire_lock();
    return ensure_unique_name_locked(client_id, audio_port);
}

std::set<std::string> discovered_clients() {
    auto lock = acquire_lock();
    pump_rx();
    if (!self_id.empty()) known_ids.insert(self_id);
    return known_ids;
}

uint16_t port_for_client(const std::string& client_id) {
    auto lock = acquire_lock();
    auto it = id_ports.find(client_id);
    if (it != id_ports.end()) return it->second;
    return 0; // Return 0 if not discovered, so we don't maliciously route to 47000
}

std::string getClientEndpoint(const std::string& client_id) {
    auto lock = acquire_lock();
    auto it = id_endpoints.find(client_id);
    if (it != id_endpoints.end() && !it->second.empty()) {
        return it->second;
    }
    uint16_t port = port_for_client(client_id);
    return std::string("127.0.0.1:") + std::to_string(port);
}

ControlResponse send_control_command(const std::string& /*server_ip*/,
                                     const std::string& command,
                                     int /*timeout_ms*/) {
    auto lock = acquire_lock();
    ensure_socket();

    if (starts_with(command, "LIST:")) {
        // command format LIST:<client>
        auto colon = command.find(':');
        if (colon != std::string::npos && colon + 1 < command.size()) {
            self_id = command.substr(colon + 1);
            ensure_unique_name_locked(self_id, self_port);
        }
        pump_rx();
        if (!self_id.empty()) known_ids.insert(self_id);
        std::string resp;
        for (auto it = known_ids.begin(); it != known_ids.end(); ++it) {
            if (it != known_ids.begin()) resp.push_back(',');
            resp += *it;
            resp.push_back('@');
            auto pit = id_ports.find(*it);
            resp += (pit != id_ports.end() ? std::to_string(pit->second) : "0");
        }
        return {true, resp};
    }

    if (starts_with(command, "REGISTER:")) {
        // command format REGISTER:<client>:<port>:<secret>
        auto parts = command.substr(9);
        auto colon1 = parts.find(':');
        if (colon1 != std::string::npos) {
            std::string id = parts.substr(0, colon1);
            auto colon2 = parts.find(':', colon1 + 1);
            if (colon2 != std::string::npos) {
                std::string portStr = parts.substr(colon1 + 1, colon2 - colon1 - 1);
                try {
                    self_port = static_cast<uint16_t>(std::stoi(portStr));
                } catch (...) {}
            }
            self_id = id;
            ensure_unique_name_locked(self_id, self_port);
            
            // Also store it directly so we don't drop the dynamic port immediately
            id_ports[self_id] = self_port;
            id_endpoints[self_id] = "127.0.0.1:" + std::to_string(self_port);
        }
        return {true, "OK"};
    }

    // default success
    return {true, "OK"};
}

std::vector<std::string> parse_client_list_response(const std::string& resp) {
    std::vector<std::string> out;
    std::stringstream ss(resp);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            auto at = item.find('@');
            std::string name = at == std::string::npos ? item : item.substr(0, at);
            quint16 port = 47000;
            if (at != std::string::npos) {
                try {
                    port = static_cast<quint16>(std::stoi(item.substr(at + 1)));
                    id_ports[name] = port;
                } catch (...) {}
            }
            id_endpoints[name] = "127.0.0.1:" + std::to_string(port);
            out.push_back(name);
        }
    }
    return out;
}

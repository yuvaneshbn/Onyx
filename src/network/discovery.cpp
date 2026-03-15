#include "discovery.h"
#include "../protocol/serializer.h"
#include <QNetworkDatagram>
#include <QDebug>

namespace network {

Discovery::Discovery(uint16_t port, QObject* parent)
    : QObject(parent), port_(port) {
    
    socket_ = new QUdpSocket(this);
    timer_ = new QTimer(this);
    
    connect(socket_, &QUdpSocket::readyRead, this, &Discovery::readPendingDatagrams);
    connect(timer_, &QTimer::timeout, this, &Discovery::sendPresence);
}

Discovery::~Discovery() {
    stop();
}

void Discovery::start(uint64_t myClientId) {
    if (socket_->state() == QAbstractSocket::BoundState) {
        return; // Already running
    }

    myClientId_ = myClientId;
    
    // Bind shared to allow multiple instances on the same machine to test
    if (!socket_->bind(QHostAddress::AnyIPv4, port_, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qWarning() << "Failed to bind Discovery socket to port" << port_;
        return;
    }

    timer_->start(5000); // Broadcast presence every 5 seconds
    sendPresence();
}

void Discovery::stop() {
    timer_->stop();
    socket_->close();
}

void Discovery::addKnownWanPeer(const QHostAddress& ip, uint16_t port) {
    knownWanPeers_.insert({ip, port});
}

void Discovery::sendPresence() {
    if (socket_->state() != QUdpSocket::BoundState) return;

    // We will repurpose Heartbeat struct as a generic PRESENCE payload for discovery
    protocol::Heartbeat hb;
    hb.client_id = myClientId_;
    hb.is_router = false; // Initial discovery assumes we're not a router yet
    hb.active_children_count = 0;
    hb.cpu_load = 0;

    auto payload = protocol::Serializer::serializeHeartbeat(hb);
    QByteArray data(reinterpret_cast<const char*>(payload.data()), payload.size());

    // 1. Send Local Subnet Broadcast
    socket_->writeDatagram(data, QHostAddress::Broadcast, port_);

    // 2. Send to known WAN peers
    for (const auto& peer : knownWanPeers_) {
        socket_->writeDatagram(data, peer.ip, peer.port);
    }
}

void Discovery::readPendingDatagrams() {
    while (socket_->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket_->receiveDatagram();
        
        QByteArray data = datagram.data();
        if (data.size() < 17) continue; // Minimum heartbeat size

        try {
            std::vector<uint8_t> buffer(data.begin(), data.end());
            protocol::Heartbeat hb = protocol::Serializer::deserializeHeartbeat(buffer);

            // Ignore our own broadcasts
            if (hb.client_id != myClientId_) {
                emit peerDiscovered(hb.client_id, datagram.senderAddress(), static_cast<uint16_t>(datagram.senderPort()));
            }

        } catch (const std::exception& e) {
            qWarning() << "Discovery deserialize error:" << e.what();
        }
    }
}

} // namespace network

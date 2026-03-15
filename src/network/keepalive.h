#ifndef KEEPALIVE_H
#define KEEPALIVE_H

#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <unordered_set>

namespace network {

// Sends periodic 1-byte "hole punch" UDP packets to prevent NAT routers from closing ports.
// Most consumer routers drop bindings after 30-60 seconds of inactivity.
class KeepAlive : public QObject {
    Q_OBJECT

public:
    explicit KeepAlive(QUdpSocket* socket, QObject* parent = nullptr);
    ~KeepAlive();

    void start(int intervalMs = 15000); // Default fire every 15s
    void stop();

    void addTarget(const QHostAddress& ip, uint16_t port);
    void removeTarget(const QHostAddress& ip, uint16_t port);

private slots:
    void sendPings();

private:
    QUdpSocket* socket_{nullptr};
    QTimer* timer_{nullptr};

    struct Target {
        QHostAddress ip;
        uint16_t port;
        bool operator==(const Target& other) const {
            return ip == other.ip && port == other.port;
        }
    };

    struct TargetHash {
        std::size_t operator()(const Target& p) const {
            return qHash(p.ip.toString() + ":" + QString::number(p.port));
        }
    };

    std::unordered_set<Target, TargetHash> targets_;
};

} // namespace network

#endif // KEEPALIVE_H

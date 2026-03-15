#ifndef DISCOVERY_H
#define DISCOVERY_H

#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QHostAddress>
#include <unordered_set>

namespace network {

// Handles LAN broadcast and WAN known-peer discovery
// Periodically broadcasts PRESENCE packets to 255.255.255.255:PORT 
// and listens for peers doing the same.
class Discovery : public QObject {
    Q_OBJECT

public:
    explicit Discovery(uint16_t port, QObject* parent = nullptr);
    ~Discovery();

    void start(uint64_t myClientId);
    void stop();

    // Add a known WAN peer to explicitly ping during discovery sweeps
    void addKnownWanPeer(const QHostAddress& ip, uint16_t port);

signals:
    // Emitted when a new peer was discovered.
    void peerDiscovered(uint64_t clientId, const QHostAddress& ip, uint16_t port);

private slots:
    void sendPresence();
    void readPendingDatagrams();

private:
    uint16_t port_;
    uint64_t myClientId_{0};
    QUdpSocket* socket_{nullptr};
    QTimer* timer_{nullptr};

    struct PeerEndpoint {
        QHostAddress ip;
        uint16_t port;
        bool operator==(const PeerEndpoint& other) const {
            return ip == other.ip && port == other.port;
        }
    };

    struct PeerEndpointHash {
        std::size_t operator()(const PeerEndpoint& p) const {
            return qHash(p.ip.toString() + ":" + QString::number(p.port));
        }
    };

    std::unordered_set<PeerEndpoint, PeerEndpointHash> knownWanPeers_;
};

} // namespace network

#endif // DISCOVERY_H

#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H

#pragma once

#include <QObject>
#include <QHostAddress>
#include <unordered_map>
#include <chrono>

namespace network {

// Tracks the state of all discovered peers and routing tree participants
class PeerManager : public QObject {
    Q_OBJECT

public:
    struct PeerInfo {
        uint64_t id;
        QHostAddress ip;
        uint16_t port;
        bool isRouter = false;
        uint32_t activeChildren = 0;
        uint32_t cpuLoad = 0;
        std::chrono::steady_clock::time_point lastSeen;
    };

    explicit PeerManager(QObject* parent = nullptr);

    void updatePeer(uint64_t id, const QHostAddress& ip, uint16_t port, 
                    bool isRouter = false, uint32_t children = 0, uint32_t cpu = 0);
    
    // Purges peers haven't been seen in > 10 seconds
    void cleanupStalePeers();

    const std::unordered_map<uint64_t, PeerInfo>& getPeers() const { return peers_; }

signals:
    void peerLost(uint64_t id);

private:
    std::unordered_map<uint64_t, PeerInfo> peers_;
};

} // namespace network

#endif // PEER_MANAGER_H

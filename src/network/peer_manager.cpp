#include "peer_manager.h"
#include <algorithm>
#include <vector>

namespace network {

PeerManager::PeerManager(QObject* parent) : QObject(parent) {}

void PeerManager::updatePeer(uint64_t id, const QHostAddress& ip, uint16_t port, 
                             bool isRouter, uint32_t children, uint32_t cpu) {
    auto now = std::chrono::steady_clock::now();
    
    auto it = peers_.find(id);
    if (it != peers_.end()) {
        it->second.ip = ip;
        it->second.port = port;
        it->second.isRouter = isRouter;
        it->second.activeChildren = children;
        it->second.cpuLoad = cpu;
        it->second.lastSeen = now;
    } else {
        peers_[id] = {id, ip, port, isRouter, children, cpu, now};
    }
}

void PeerManager::cleanupStalePeers() {
    auto now = std::chrono::steady_clock::now();
    const auto timeout = std::chrono::seconds(10);
    
    std::vector<uint64_t> toRemove;
    for (const auto& [id, info] : peers_) {
        if (now - info.lastSeen > timeout) {
            toRemove.push_back(id);
        }
    }

    for (uint64_t id : toRemove) {
        peers_.erase(id);
        emit peerLost(id);
    }
}

} // namespace network

#include "failover_manager.h"
#include <QDebug>

namespace network {

FailoverManager::FailoverManager(PeerManager* peerManager, QObject* parent)
    : QObject(parent), peerManager_(peerManager) {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &FailoverManager::checkParentHealth);
}

void FailoverManager::setParentRouter(uint64_t routerId) {
    parentRouterId_ = routerId;
}

void FailoverManager::startMonitoring(int checkIntervalMs, int timeoutMs) {
    timeoutMs_ = timeoutMs;
    timer_->start(checkIntervalMs);
}

void FailoverManager::stopMonitoring() {
    timer_->stop();
}

void FailoverManager::checkParentHealth() {
    if (parentRouterId_ == 0) return; // No parent to monitor

    const auto& peers = peerManager_->getPeers();
    auto it = peers.find(parentRouterId_);
    
    // If the peer is entirely gone from PeerManager (already purged), or hasn't been seen in `timeoutMs_`
    if (it == peers.end()) {
        qWarning() << "Parent router" << parentRouterId_ << "purged. Triggering failover!";
        parentRouterId_ = 0; // Prevent spamming
        emit parentRouterFailed();
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastSeen = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second.lastSeen).count();

    if (timeSinceLastSeen > timeoutMs_) {
        qWarning() << "Parent router" << parentRouterId_ << "timeout" << timeSinceLastSeen << "ms. Triggering failover!";
        parentRouterId_ = 0; // Prevent spamming
        emit parentRouterFailed();
    }
}

} // namespace network

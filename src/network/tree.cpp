#include "tree.h"
#include <QDebug>

namespace network {

Tree::Tree(PeerManager* peerManager, uint64_t myClientId, QObject* parent)
    : QObject(parent), peerManager_(peerManager), myClientId_(myClientId) {
    
    electionTimer_ = new QTimer(this);
    connect(electionTimer_, &QTimer::timeout, this, &Tree::evaluateElection);
}

void Tree::startElection() {
    // Evaluate the tree every 5 seconds to ensure we have the best router
    electionTimer_->start(5000);
    evaluateElection();
}

void Tree::joinTree(uint64_t parentRouterId) {
    if (parentId_ == parentRouterId) return;

    parentId_ = parentRouterId;
    role_ = Role::Leaf; // Assume leaf until children connect to us
    
    emit connectedToParent(parentId_);
    emit roleChanged(role_);
}

void Tree::evaluateElection() {
    uint64_t bestRouterId = determineBestRouter();
    
    if (bestRouterId == myClientId_) {
        if (role_ != Role::RootRouter) {
            role_ = Role::RootRouter;
            parentId_ = 0;
            qInfo() << "I am the new Root Router! (ID:" << myClientId_ << ")";
            emit roleChanged(role_);
        }
    } else {
        if (parentId_ != bestRouterId) {
            qInfo() << "Switching to better Router ID:" << bestRouterId;
            joinTree(bestRouterId);
        }
    }
}

uint64_t Tree::determineBestRouter() const {
    const auto& peers = peerManager_->getPeers();
    
    if (peers.empty()) return myClientId_; // I am the only one

    uint64_t bestId = myClientId_;
    // Lower means better (Mock composite score)
    // CPU load is paramount, then number of active children (lower is better for balancing)
    uint64_t bestScore = (0 /* mock CPU */ * 1000) + childrenIds_.size();

    for (const auto& [id, info] : peers) {
        uint64_t score = (info.cpuLoad * 1000) + info.activeChildren;
        
        // Tie breaker is the lowest Client ID natively
        if (score < bestScore || (score == bestScore && id < bestId)) {
            bestScore = score;
            bestId = id;
        }
    }

    return bestId;
}

} // namespace network

#ifndef TREE_H
#define TREE_H

#pragma once

#include "peer_manager.h"
#include <QObject>
#include <QTimer>
#include <vector>

namespace network {

// Manages Distributed Router Election and BFS Tree Construction.
// Every client launches as a "Leaf".
// During Election, the client with the lowest CPU load and highest bandwidth
// becomes the "Root Router" for a subnet/WAN cluster.
class Tree : public QObject {
    Q_OBJECT

public:
    enum class Role {
        Leaf,
        SubRouter,
        RootRouter
    };

    explicit Tree(PeerManager* peerManager, uint64_t myClientId, QObject* parent = nullptr);

    void startElection();
    void joinTree(uint64_t parentRouterId);

    Role getCurrentRole() const { return role_; }
    uint64_t getParentId() const { return parentId_; }
    const std::vector<uint64_t>& getChildren() const { return childrenIds_; }

signals:
    void roleChanged(Role newRole);
    void connectedToParent(uint64_t parentId);
    void childJoined(uint64_t childId);

private slots:
    void evaluateElection();

private:
    PeerManager* peerManager_{nullptr};
    uint64_t myClientId_{0};
    Role role_{Role::Leaf};
    
    uint64_t parentId_{0};
    std::vector<uint64_t> childrenIds_;
    
    QTimer* electionTimer_{nullptr};

    uint64_t determineBestRouter() const;
};

} // namespace network

#endif // TREE_H

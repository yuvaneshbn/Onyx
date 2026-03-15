#ifndef FAILOVER_MANAGER_H
#define FAILOVER_MANAGER_H

#pragma once

#include "peer_manager.h"
#include <QObject>
#include <QTimer>
#include <memory>

namespace network {

// Watches the active Parent Router for timeouts.
// If the parent dies, triggers a "Subtree Re-election" process where children 
// bid to become the new router based on CPU/Bandwidth metrics.
class FailoverManager : public QObject {
    Q_OBJECT

public:
    explicit FailoverManager(PeerManager* peerManager, QObject* parent = nullptr);

    // Set the ID of the router we are currently connected to.
    void setParentRouter(uint64_t routerId);
    
    void startMonitoring(int checkIntervalMs = 2000, int timeoutMs = 5000);
    void stopMonitoring();

signals:
    // Emitted when the parent router is confirmed dead.
    // The system should switch to "Election Mode" upon receiving this.
    void parentRouterFailed();

private slots:
    void checkParentHealth();

private:
    PeerManager* peerManager_{nullptr};
    QTimer* timer_{nullptr};
    
    uint64_t parentRouterId_{0};
    int timeoutMs_{5000};
};

} // namespace network

#endif // FAILOVER_MANAGER_H

#ifndef ROUTER_LOAD_H
#define ROUTER_LOAD_H

#pragma once

#include <QObject>
#include <QTimer>

namespace network {

// Monitors system CPU usage and network bandwidth (mocked for cross-platform simplicity)
// This score is broadcasted in Heartbeats to influence Router Election.
class RouterLoad : public QObject {
    Q_OBJECT

public:
    explicit RouterLoad(QObject* parent = nullptr);

    void startMonitoring(int intervalMs = 2000);
    void stopMonitoring();

    uint32_t getCurrentCpuLoad() const { return currentCpuLoad_; }

signals:
    void loadUpdated(uint32_t cpuLoad);

private slots:
    void measureLoad();

private:
    QTimer* timer_{nullptr};
    uint32_t currentCpuLoad_{0};
    
    // In a real C++ app you'd read /proc/stat on Linux, or Performance Counters on Windows.
    // For scaffolding, we generate a synthetic load.
};

} // namespace network

#endif // ROUTER_LOAD_H

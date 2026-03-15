#include "router_load.h"
#include <QRandomGenerator>

namespace network {

RouterLoad::RouterLoad(QObject* parent) : QObject(parent) {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &RouterLoad::measureLoad);
}

void RouterLoad::startMonitoring(int intervalMs) {
    timer_->start(intervalMs);
    measureLoad();
}

void RouterLoad::stopMonitoring() {
    timer_->stop();
}

void RouterLoad::measureLoad() {
    // Synthetic load generation: Random variance around a baseline
    // 0-100 scale.
    int variance = QRandomGenerator::global()->bounded(-5, 5);
    int newLoad = std::clamp(static_cast<int>(currentCpuLoad_) + variance, 5, 95);
    
    currentCpuLoad_ = static_cast<uint32_t>(newLoad);
    emit loadUpdated(currentCpuLoad_);
}

} // namespace network

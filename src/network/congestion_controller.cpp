#include "congestion_controller.h"
#include <QDebug>
#include <algorithm>

namespace network {

CongestionController::CongestionController(QObject* parent) 
    : QObject(parent) {
    lastReductionTime_ = std::chrono::steady_clock::now();
}

void CongestionController::processRtcpReport(uint32_t ssrc, float fractionLost, uint32_t jitterMs) {
    Q_UNUSED(ssrc);

    auto now = std::chrono::steady_clock::now();
    
    // Hold-off timer: Don't reduce bandwidth more than once every 2 seconds
    auto msSinceLastReduction = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastReductionTime_).count();

    // 1. Check for Congestion (Loss > 2% or Jitter > 100ms)
    if (fractionLost > 0.02f || jitterMs > 100) {
        if (msSinceLastReduction > 2000) {
            decreaseBitrate();
            lastReductionTime_ = now;
        }
    } 
    // 2. Check for Recovery (Loss < 1% and Jitter < 30ms)
    else if (fractionLost < 0.01f && jitterMs < 30) {
        // Slowly ramp up if we've been stable for 3 seconds
        if (msSinceLastReduction > 3000) {
            increaseBitrate();
            lastReductionTime_ = now; // Reset timer to avoid accelerating too fast
        }
    }
}

void CongestionController::decreaseBitrate() {
    // Multiplicative Decrease (usually 0.85x)
    int newBitrate = static_cast<int>(currentBitrateKbps_ * 0.85);
    newBitrate = std::max(newBitrate, minBitrateKbps_);
    
    if (newBitrate != currentBitrateKbps_) {
        currentBitrateKbps_ = newBitrate;
        qInfo() << "Congestion detected! Reducing bitrate to" << currentBitrateKbps_ << "kbps";
        emit targetBitrateChanged(currentBitrateKbps_);
    }
}

void CongestionController::increaseBitrate() {
    // Additive Increase (+2 kbps)
    int newBitrate = currentBitrateKbps_ + 2;
    newBitrate = std::min(newBitrate, maxBitrateKbps_);
    
    if (newBitrate != currentBitrateKbps_) {
        currentBitrateKbps_ = newBitrate;
        qInfo() << "Network stable. Increasing bitrate to" << currentBitrateKbps_ << "kbps";
        emit targetBitrateChanged(currentBitrateKbps_);
    }
}

} // namespace network

#ifndef CONGESTION_CONTROLLER_H
#define CONGESTION_CONTROLLER_H

#pragma once

#include <QObject>
#include <chrono>

namespace network {

// Basic Google Congestion Control Algorithm (AIMD - Additive Increase Multiplicative Decrease).
// Based on RTCP receiver reports indicating packet loss or jitter, 
// we signal the Audio Codec (Opus) to drop bitrate or increase frame size to avoid dropping packets.
class CongestionController : public QObject {
    Q_OBJECT

public:
    explicit CongestionController(QObject* parent = nullptr);

    void processRtcpReport(uint32_t ssrc, float fractionLost, uint32_t jitterMs);

signals:
    // Emitted when the target encoding bitrate should change
    void targetBitrateChanged(int newBitrateKbps);

private:
    int currentBitrateKbps_{48}; // Default Opus high-quality voice
    int maxBitrateKbps_{64};
    int minBitrateKbps_{12};
    
    std::chrono::steady_clock::time_point lastReductionTime_;

    void decreaseBitrate();
    void increaseBitrate();
};

} // namespace network

#endif // CONGESTION_CONTROLLER_H

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#pragma once

#include "codec.h"
#include "../utils/vad.h"
#include "../utils/jitter_buffer.h"
#include <QObject>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <opus/opus.h>

namespace audio {

// Orchestrates the entire Audio Pipeline.
// In a real app, this would wrap miniaudio or PortAudio to read/write hardware buffers.
class Engine : public QObject {
    Q_OBJECT

public:
    explicit Engine(QObject* parent = nullptr);
    ~Engine() override;

    // Start/Stop Mic Capture and Speaker Playback
    void start();
    void stop();
    
    // Accept an incoming RTP packet from the Network layer
    void receiveRemotePacket(uint32_t ssrc, uint16_t seqNum, const std::vector<uint8_t>& payload);

    // Provide the hardware PCM buffer (mocked for now)
    void processCaptureBuffer(const float* pcm, int numSamples);
    void processPlaybackBuffer(float* pcm, int numSamples);

signals:
    // Emitted when a fully encoded Opus packet is ready to send over UDP
    void localPacketReady(const std::vector<uint8_t>& payload);

private:
    Codec codec_;
    Vad vad_;
    
    // One jitter buffer + one Opus decoder per remote speaker
    struct RemoteStream {
        std::unique_ptr<utils::JitterBuffer> jb;
        OpusDecoder* decoder{nullptr};
    };
    std::map<uint32_t, RemoteStream> remoteStreams_;
    std::mutex jbMutex_;

    bool isRunning_{false};
    uint16_t localSeqNum_{0};

    // Pre-processing steps
    void applyAgc(float* pcm, int numSamples);
    void applyNs(float* pcm, int numSamples);
    void applyAec(float* pcm, const float* referencePcm, int numSamples);
};

} // namespace audio

#endif // AUDIO_ENGINE_H

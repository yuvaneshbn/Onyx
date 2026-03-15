#include "engine.h"
#include <QDebug>
#include <cstring>
#include <algorithm>
#include <cmath>

namespace audio {

Engine::Engine(QObject* parent) 
    : QObject(parent), codec_(48000, 1), vad_(-45.0f) {
}

Engine::~Engine() {
    stop();
    std::lock_guard<std::mutex> lock(jbMutex_);
    for (auto& [ssrc, rs] : remoteStreams_) {
        if (rs.decoder) {
            opus_decoder_destroy(rs.decoder);
            rs.decoder = nullptr;
        }
    }
    remoteStreams_.clear();
}

void Engine::start() {
    if (isRunning_) return;
    isRunning_ = true;
    qInfo() << "Audio Engine started.";
}

void Engine::stop() {
    isRunning_ = false;
}

void Engine::receiveRemotePacket(uint32_t ssrc, uint16_t seqNum, const std::vector<uint8_t>& payload) {
    if (!isRunning_) return;
    
    std::lock_guard<std::mutex> lock(jbMutex_);

    auto it = remoteStreams_.find(ssrc);
    if (it == remoteStreams_.end()) {
        // Create a new stream entry with its own jitter buffer AND its own Opus decoder
        RemoteStream rs;
        rs.jb = std::make_unique<utils::JitterBuffer>(10);

        int err = OPUS_OK;
        rs.decoder = opus_decoder_create(48000, 1, &err);
        if (err != OPUS_OK || !rs.decoder) {
            qWarning() << "Failed to create per-SSRC Opus decoder for SSRC" << ssrc << ":" << opus_strerror(err);
        } else {
            qInfo() << "Successfully created Opus decoder for SSRC:" << ssrc;
        }

        it = remoteStreams_.emplace(ssrc, std::move(rs)).first;
        qInfo() << "New remote speaker SSRC:" << ssrc;
    }
    it->second.jb->push(seqNum, payload);
    qDebug() << "Pushed packet to JB. Seq:" << seqNum << "Payload size:" << payload.size() << "SSRC:" << ssrc;
}

void Engine::processCaptureBuffer(const float* pcm, int numSamples) {
    if (!isRunning_) return;

    // Copy buffer for processing (AGC, NS, AEC)
    std::vector<float> processedBuffer(pcm, pcm + numSamples);

    applyAec(processedBuffer.data(), nullptr, numSamples);
    applyNs(processedBuffer.data(), numSamples);
    applyAgc(processedBuffer.data(), numSamples);

    // Voice Activity Detection - only send if speech detected
    if (!vad_.checkActivity(processedBuffer.data(), numSamples)) {
        return; // Silence detected, skip encode/send
    }

    auto payload = codec_.encode(processedBuffer.data(), numSamples);
    emit localPacketReady(payload);
    localSeqNum_++;
}

void Engine::processPlaybackBuffer(float* pcm, int numSamples) {
    if (!isRunning_) return;

    std::fill_n(pcm, numSamples, 0.0f);

    std::vector<std::vector<float>> decodedStreams;

    {
        std::lock_guard<std::mutex> lock(jbMutex_);
        for (auto& [ssrc, rs] : remoteStreams_) {
            if (!rs.decoder || !rs.jb) continue;

            auto payload = rs.jb->pop();

            // Decode using the per-SSRC decoder directly
            std::vector<float> decoded(numSamples, 0.0f);
            int decodedSamples = 0;

            if (payload.empty()) {
                // Packet Loss Concealment
                decodedSamples = opus_decode_float(rs.decoder, nullptr, 0,
                                                   decoded.data(), numSamples, 0);
            } else {
                decodedSamples = opus_decode_float(rs.decoder,
                                                   payload.data(),
                                                   static_cast<opus_int32>(payload.size()),
                                                   decoded.data(), numSamples, 0);
            }

            if (decodedSamples < 0) {
                qWarning() << "Opus decode error for SSRC" << ssrc << ":" << opus_strerror(decodedSamples);
                decoded.assign(numSamples, 0.0f);
            } else {
                qDebug() << "Decoded" << decodedSamples << "samples for SSRC" << ssrc;
            }

            decodedStreams.push_back(std::move(decoded));
        }
    }

    if (decodedStreams.empty()) return;

    // Mix all remote streams
    float mixPeak = 0.0f;
    for (int i = 0; i < numSamples; ++i) {
        float mixedSample = 0.0f;
        for (const auto& stream : decodedStreams) {
            mixedSample += stream[i];
        }
        pcm[i] = std::clamp(mixedSample, -1.0f, 1.0f);
        mixPeak = std::max(mixPeak, std::abs(pcm[i]));
    }
}

// Scaffolded Audio Processing algorithms
void Engine::applyAgc(float* /*pcm*/, int /*numSamples*/) {}
void Engine::applyNs(float* /*pcm*/, int /*numSamples*/) {}
void Engine::applyAec(float* /*pcm*/, const float* /*ref*/, int /*numSamples*/) {}

} // namespace audio

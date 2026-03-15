#ifndef CODEC_H
#define CODEC_H

#pragma once

#include <vector>
#include <cstdint>
#include <opus/opus.h>

namespace audio {

// Wrapper around the Opus codec
// Handles encoding raw PCM audio into Opus frames, and decoding them back.
// Designed for 48kHz, 20ms frames, Stereo/Mono.
class Codec {
public:
    Codec(int sampleRate = 48000, int channels = 1);
    ~Codec();

    // Set target bitrate dynamically (called by CongestionController)
    void setBitrate(int bitrateKbps);

    // Encodes PCM float data into Opus bytes
    std::vector<uint8_t> encode(const float* pcm, int frameSizePerChannel);

    // Decodes Opus bytes back into PCM float data. 
    // If 'packet' is empty, triggers Packet Loss Concealment (PLC).
    std::vector<float> decode(const std::vector<uint8_t>& packet, int frameSizePerChannel);

private:
    int sampleRate_;
    int channels_;
    int bitrateKbps_;

    OpusEncoder* encoder_{nullptr};
    OpusDecoder* decoder_{nullptr};
};

} // namespace audio

#endif // CODEC_H

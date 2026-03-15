#include "codec.h"
#include <QDebug>
#include <cstring>
#include <stdexcept>

namespace audio {

Codec::Codec(int sampleRate, int channels)
    : sampleRate_(sampleRate), channels_(channels), bitrateKbps_(48) {
    
    int err = OPUS_OK;
    encoder_ = opus_encoder_create(sampleRate_, channels_, OPUS_APPLICATION_VOIP, &err);
    if (err != OPUS_OK) {
        qWarning() << "Failed to create Opus encoder:" << opus_strerror(err);
    }
    
    decoder_ = opus_decoder_create(sampleRate_, channels_, &err);
    if (err != OPUS_OK) {
        qWarning() << "Failed to create Opus decoder:" << opus_strerror(err);
    }

    qInfo() << "Initialized Opus Codec at" << sampleRate_ << "Hz," << channels_ << "channels.";
    setBitrate(bitrateKbps_);
}

Codec::~Codec() {
    if (encoder_) opus_encoder_destroy(encoder_);
    if (decoder_) opus_decoder_destroy(decoder_);
}

void Codec::setBitrate(int bitrateKbps) {
    bitrateKbps_ = bitrateKbps;
    if (encoder_) {
        opus_encoder_ctl(encoder_, OPUS_SET_BITRATE(bitrateKbps_ * 1000));
    }
}

std::vector<uint8_t> Codec::encode(const float* pcm, int frameSizePerChannel) {
    std::vector<uint8_t> packet;
    if (!encoder_ || pcm == nullptr || frameSizePerChannel <= 0) return packet;

    // A theoretical max size for Opus packet is 4000 bytes.
    std::vector<uint8_t> rawBuffer(4000);
    int bytesEncoded = opus_encode_float(encoder_, pcm, frameSizePerChannel, rawBuffer.data(), static_cast<opus_int32>(rawBuffer.size()));
    
    if (bytesEncoded > 0) {
        packet.assign(rawBuffer.begin(), rawBuffer.begin() + bytesEncoded);
    } else {
         qWarning() << "Opus encode failed:" << opus_strerror(bytesEncoded);
    }

    return packet;
}

std::vector<float> Codec::decode(const std::vector<uint8_t>& packet, int frameSizePerChannel) {
    std::vector<float> pcm;
    pcm.resize(frameSizePerChannel * channels_, 0.0f);

    if (!decoder_) return pcm;

    int decodedSamples = 0;
    if (packet.empty()) {
        // PLC: Packet Loss Concealment
        decodedSamples = opus_decode_float(decoder_, nullptr, 0, pcm.data(), frameSizePerChannel, 0);
    } else {
        decodedSamples = opus_decode_float(decoder_, packet.data(), static_cast<opus_int32>(packet.size()), pcm.data(), frameSizePerChannel, 0);
    }

    if (decodedSamples < 0) {
        qWarning() << "Opus decode failed:" << opus_strerror(decodedSamples);
        std::fill(pcm.begin(), pcm.end(), 0.0f); // Fallback to silence
    }

    return pcm;
}

} // namespace audio

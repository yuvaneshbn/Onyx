#include "OpusAudioPipeline.h"
#include <QDebug>

namespace Onyx {

OpusAudioPipeline::OpusAudioPipeline(int sampleRate, int channels, int bitrate)
    : m_sampleRate(sampleRate),
      m_channels(channels),
      m_bitrate(bitrate) {
}

OpusAudioPipeline::~OpusAudioPipeline() {
    cleanup();
}

bool OpusAudioPipeline::initialize() {
    cleanup();

    int err = OPUS_OK;

    // Create Opus Encoder
    m_encoder = opus_encoder_create(m_sampleRate, m_channels, OPUS_APPLICATION_VOIP, &err);
    if (err != OPUS_OK || !m_encoder) {
        qWarning() << "Failed to create Opus encoder, error code:" << err;
        return false;
    }

    opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(m_bitrate));
    opus_encoder_ctl(m_encoder, OPUS_SET_VBR(1));
    opus_encoder_ctl(m_encoder, OPUS_SET_COMPLEXITY(8));
    opus_encoder_ctl(m_encoder, OPUS_SET_INBAND_FEC(1));
    opus_encoder_ctl(m_encoder, OPUS_SET_PACKET_LOSS_PERC(5));

    // Create Opus Decoder
    m_decoder = opus_decoder_create(m_sampleRate, m_channels, &err);
    if (err != OPUS_OK || !m_decoder) {
        qWarning() << "Failed to create Opus decoder, error code:" << err;
        cleanup();
        return false;
    }

    return true;
}

void OpusAudioPipeline::cleanup() {
    if (m_encoder) {
        opus_encoder_destroy(m_encoder);
        m_encoder = nullptr;
    }
    if (m_decoder) {
        opus_decoder_destroy(m_decoder);
        m_decoder = nullptr;
    }
}

int OpusAudioPipeline::encodeFrame(const qint16* pcmData, int frameSizeSamples, QByteArray& outOpus) {
    if (!m_encoder || !pcmData || frameSizeSamples <= 0) {
        return -1;
    }

    // Allocate buffer for worst-case Opus frame (4000 bytes is more than enough for a 20ms frame)
    outOpus.resize(4000);
    opus_int32 bytesEncoded = opus_encode(m_encoder,
                                          reinterpret_cast<const opus_int16*>(pcmData),
                                          frameSizeSamples,
                                          reinterpret_cast<unsigned char*>(outOpus.data()),
                                          outOpus.size());

    if (bytesEncoded < 0) {
        qWarning() << "Opus encoding failed with error:" << bytesEncoded;
        outOpus.clear();
        return -1;
    }

    outOpus.resize(bytesEncoded);
    return bytesEncoded;
}

int OpusAudioPipeline::decodeFrame(const QByteArray& opusData, QVector<qint16>& outPcm, int frameSizeSamples) {
    if (!m_decoder) {
        return -1;
    }

    outPcm.resize(frameSizeSamples * m_channels);
    int decodedSamples = opus_decode(m_decoder,
                                     reinterpret_cast<const unsigned char*>(opusData.constData()),
                                     opusData.size(),
                                     reinterpret_cast<opus_int16*>(outPcm.data()),
                                     frameSizeSamples,
                                     0); // 0 = no FEC decode

    if (decodedSamples < 0) {
        qWarning() << "Opus decoding failed with error:" << decodedSamples;
        outPcm.clear();
        return -1;
    }

    outPcm.resize(decodedSamples * m_channels);
    return decodedSamples;
}

int OpusAudioPipeline::decodeLostFrame(QVector<qint16>& outPcm, int frameSizeSamples) {
    if (!m_decoder) {
        return -1;
    }

    outPcm.resize(frameSizeSamples * m_channels);
    // Passing NULL to opus_decode invokes internal PLC (Packet Loss Concealment)
    int decodedSamples = opus_decode(m_decoder,
                                     nullptr,
                                     0,
                                     reinterpret_cast<opus_int16*>(outPcm.data()),
                                     frameSizeSamples,
                                     1); // decode FEC if available

    if (decodedSamples < 0) {
        outPcm.clear();
        return -1;
    }

    outPcm.resize(decodedSamples * m_channels);
    return decodedSamples;
}

void OpusAudioPipeline::setBitrate(int bitrate) {
    m_bitrate = bitrate;
    if (m_encoder) {
        opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(m_bitrate));
    }
}

} // namespace Onyx

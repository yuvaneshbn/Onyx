#ifndef OPUS_AUDIO_PIPELINE_H
#define OPUS_AUDIO_PIPELINE_H

#include <QByteArray>
#include <QVector>
#include <opus.h>
#include "../core/NetworkConfig.h"

namespace Onyx {

class OpusAudioPipeline {
public:
    OpusAudioPipeline(int sampleRate = AUDIO_SAMPLE_RATE, int channels = AUDIO_CHANNELS, int bitrate = 64000);
    ~OpusAudioPipeline();

    bool initialize();
    void cleanup();

    // Encodes PCM (16-bit signed, mono/stereo) into Opus packet
    int encodeFrame(const qint16* pcmData, int frameSizeSamples, QByteArray& outOpus);

    // Decodes Opus packet back into PCM (16-bit signed)
    int decodeFrame(const QByteArray& opusData, QVector<qint16>& outPcm, int frameSizeSamples = OPUS_SAMPLES_PER_FRAME);

    // Packet Loss Concealment (PLC): decodes missing frame
    int decodeLostFrame(QVector<qint16>& outPcm, int frameSizeSamples = OPUS_SAMPLES_PER_FRAME);

    int sampleRate() const { return m_sampleRate; }
    int channels() const { return m_channels; }
    int bitrate() const { return m_bitrate; }

    void setBitrate(int bitrate);

private:
    int m_sampleRate;
    int m_channels;
    int m_bitrate;

    OpusEncoder* m_encoder = nullptr;
    OpusDecoder* m_decoder = nullptr;
};

} // namespace Onyx

#endif // OPUS_AUDIO_PIPELINE_H

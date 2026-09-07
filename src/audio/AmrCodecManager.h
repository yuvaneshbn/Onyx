#ifndef AMR_CODEC_MANAGER_H
#define AMR_CODEC_MANAGER_H

#include <QByteArray>
#include <QVector>

namespace Onyx {

class AmrCodecManager {
public:
    AmrCodecManager();
    ~AmrCodecManager();

    bool initialize();
    void cleanup();

    // Encodes 16 kHz 16-bit PCM (320 samples) to AMR-WB frame
    int encodeFrame(const qint16* pcm16k, int samples, QByteArray& outAmrWb);

    // Decodes AMR-WB frame to 16 kHz 16-bit PCM (320 samples)
    int decodeFrame(const QByteArray& amrWbData, QVector<qint16>& outPcm16k);

    // Resampling utilities between 48 kHz (system rate) and 16 kHz (AMR-WB rate)
    static QVector<qint16> resample48To16(const qint16* src48k, int samples48k);
    static QVector<qint16> resample16To48(const qint16* src16k, int samples16k);

    bool isInitialized() const { return m_initialized; }

private:
    void* m_wbEncoderState = nullptr; // vo-amrwbenc handle
    void* m_wbDecoderState = nullptr; // opencore-amrwb handle
    bool m_initialized = false;
};

} // namespace Onyx

#endif // AMR_CODEC_MANAGER_H

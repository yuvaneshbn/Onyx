#include "AmrCodecManager.h"
#include <enc_if.h>
#include <dec_if.h>
#include <QDebug>

namespace Onyx {

AmrCodecManager::AmrCodecManager() {
}

AmrCodecManager::~AmrCodecManager() {
    cleanup();
}

bool AmrCodecManager::initialize() {
    cleanup();

    m_wbEncoderState = E_IF_init();
    if (!m_wbEncoderState) {
        qWarning() << "Failed to initialize vo-amrwbenc encoder";
        return false;
    }

    m_wbDecoderState = D_IF_init();
    if (!m_wbDecoderState) {
        qWarning() << "Failed to initialize opencore-amrwb decoder";
        cleanup();
        return false;
    }

    m_initialized = true;
    return true;
}

void AmrCodecManager::cleanup() {
    if (m_wbEncoderState) {
        E_IF_exit(m_wbEncoderState);
        m_wbEncoderState = nullptr;
    }
    if (m_wbDecoderState) {
        D_IF_exit(m_wbDecoderState);
        m_wbDecoderState = nullptr;
    }
    m_initialized = false;
}

int AmrCodecManager::encodeFrame(const qint16* pcm16k, int samples, QByteArray& outAmrWb) {
    if (!m_wbEncoderState || !pcm16k || samples < 320) {
        return -1;
    }

    // AMR-WB frame buffer max size is 61 bytes (for Mode 8: 23.85 kbps)
    outAmrWb.resize(64);
    // Mode 8: 23.85 kbps (best quality), dtx = 0 (disabled)
    int bytesEncoded = E_IF_encode(m_wbEncoderState, 8,
                                   reinterpret_cast<const short*>(pcm16k),
                                   reinterpret_cast<unsigned char*>(outAmrWb.data()),
                                   0);

    if (bytesEncoded <= 0) {
        outAmrWb.clear();
        return -1;
    }

    outAmrWb.resize(bytesEncoded);
    return bytesEncoded;
}

int AmrCodecManager::decodeFrame(const QByteArray& amrWbData, QVector<qint16>& outPcm16k) {
    if (!m_wbDecoderState || amrWbData.isEmpty()) {
        return -1;
    }

    outPcm16k.resize(320); // 320 samples (20 ms at 16 kHz)
    D_IF_decode(m_wbDecoderState,
                reinterpret_cast<const unsigned char*>(amrWbData.constData()),
                reinterpret_cast<short*>(outPcm16k.data()),
                _good_frame);

    return 320;
}

QVector<qint16> AmrCodecManager::resample48To16(const qint16* src48k, int samples48k) {
    // Integer decimation by factor of 3 (48000 / 3 = 16000)
    int outCount = samples48k / 3;
    QVector<qint16> out(outCount);
    for (int i = 0; i < outCount; ++i) {
        // Average 3 adjacent samples as a simple low-pass anti-aliasing filter
        int sum = src48k[i * 3] + src48k[i * 3 + 1] + src48k[i * 3 + 2];
        out[i] = static_cast<qint16>(sum / 3);
    }
    return out;
}

QVector<qint16> AmrCodecManager::resample16To48(const qint16* src16k, int samples16k) {
    // Linear interpolation expansion by factor of 3 (16000 * 3 = 48000)
    int outCount = samples16k * 3;
    QVector<qint16> out(outCount);
    for (int i = 0; i < samples16k; ++i) {
        qint16 current = src16k[i];
        qint16 next = (i + 1 < samples16k) ? src16k[i + 1] : current;
        out[i * 3 + 0] = current;
        out[i * 3 + 1] = static_cast<qint16>((current * 2 + next) / 3);
        out[i * 3 + 2] = static_cast<qint16>((current + next * 2) / 3);
    }
    return out;
}

} // namespace Onyx

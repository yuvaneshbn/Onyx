#include "VpxVideoPipeline.h"
#include <QDebug>

namespace Onyx {

VpxVideoPipeline::VpxVideoPipeline(VpxCodec codec, int width, int height, int fps)
    : m_codec(codec),
      m_width(width),
      m_height(height),
      m_fps(fps) {
}

VpxVideoPipeline::~VpxVideoPipeline() {
    cleanup();
}

bool VpxVideoPipeline::initialize() {
    cleanup();

    vpx_codec_iface_t* encoderInterface = (m_codec == VP8) ? vpx_codec_vp8_cx() : vpx_codec_vp9_cx();
    vpx_codec_iface_t* decoderInterface = (m_codec == VP8) ? vpx_codec_vp8_dx() : vpx_codec_vp9_dx();

    // 1. Configure and initialize Encoder
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(encoderInterface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        qWarning() << "Failed to get default VPX encoder config:" << vpx_codec_err_to_string(res);
        return false;
    }

    cfg.g_w = m_width;
    cfg.g_h = m_height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = m_fps;
    cfg.rc_target_bitrate = m_bitrateKbps;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
    cfg.g_lag_in_frames = 0; // Low latency real-time mode
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 56;
    cfg.rc_undershoot_pct = 100;
    cfg.rc_overshoot_pct = 15;
    cfg.rc_buf_sz = 1000;
    cfg.rc_buf_initial_sz = 500;
    cfg.rc_buf_optimal_sz = 600;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = m_fps * 2; // Keyframe every 2 seconds

    res = vpx_codec_enc_init(&m_encoderCtx, encoderInterface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        qWarning() << "Failed to initialize VPX encoder:" << vpx_codec_err_to_string(res);
        return false;
    }

    // Set CPU usage to real-time deadline
    vpx_codec_control(&m_encoderCtx, VP8E_SET_CPUUSED, 8); // Fast encoding for real-time LAN
    m_encoderInitialized = true;

    // 2. Allocate reusable raw YUV420 image
    m_rawImage = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, m_width, m_height, 1);
    if (!m_rawImage) {
        qWarning() << "Failed to allocate VPX raw image buffer";
        cleanup();
        return false;
    }

    // 3. Initialize Decoder
    vpx_codec_dec_cfg_t decCfg = {0, 0, 0};
    decCfg.threads = 1;
    res = vpx_codec_dec_init(&m_decoderCtx, decoderInterface, &decCfg, 0);
    if (res != VPX_CODEC_OK) {
        qWarning() << "Failed to initialize VPX decoder:" << vpx_codec_err_to_string(res);
        cleanup();
        return false;
    }

    m_decoderInitialized = true;
    m_frameCounter = 0;
    return true;
}

void VpxVideoPipeline::cleanup() {
    if (m_encoderInitialized) {
        vpx_codec_destroy(&m_encoderCtx);
        m_encoderInitialized = false;
    }
    if (m_decoderInitialized) {
        vpx_codec_destroy(&m_decoderCtx);
        m_decoderInitialized = false;
    }
    if (m_rawImage) {
        vpx_img_free(m_rawImage);
        m_rawImage = nullptr;
    }
}

void VpxVideoPipeline::setBitrate(int kbps) {
    m_bitrateKbps = kbps;
    if (m_encoderInitialized) {
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_enc_config_default((m_codec == VP8) ? vpx_codec_vp8_cx() : vpx_codec_vp9_cx(), &cfg, 0);
        cfg.rc_target_bitrate = m_bitrateKbps;
        vpx_codec_enc_config_set(&m_encoderCtx, &cfg);
    }
}

bool VpxVideoPipeline::encodeYuvFrame(const quint8* yPlane, const quint8* uPlane, const quint8* vPlane,
                                     int yStride, int uvStride, QByteArray& outPacket, bool forceKeyframe) {
    if (!m_encoderInitialized || !m_rawImage) return false;

    // Copy planar YUV into vpx_image_t
    for (int y = 0; y < m_height; ++y) {
        memcpy(m_rawImage->planes[VPX_PLANE_Y] + y * m_rawImage->stride[VPX_PLANE_Y],
               yPlane + y * yStride, m_width);
    }
    for (int y = 0; y < m_height / 2; ++y) {
        memcpy(m_rawImage->planes[VPX_PLANE_U] + y * m_rawImage->stride[VPX_PLANE_U],
               uPlane + y * uvStride, m_width / 2);
        memcpy(m_rawImage->planes[VPX_PLANE_V] + y * m_rawImage->stride[VPX_PLANE_V],
               vPlane + y * uvStride, m_width / 2);
    }

    vpx_enc_frame_flags_t flags = forceKeyframe ? VPX_EFLAG_FORCE_KF : 0;
    vpx_codec_err_t res = vpx_codec_encode(&m_encoderCtx, m_rawImage, m_frameCounter++, 1, flags, VPX_DL_REALTIME);
    if (res != VPX_CODEC_OK) {
        qWarning() << "vpx_codec_encode failed:" << vpx_codec_err_to_string(res);
        return false;
    }

    outPacket.clear();
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    vpx_codec_iter_t iter = nullptr;

    while ((pkt = vpx_codec_get_cx_data(&m_encoderCtx, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            outPacket.append(reinterpret_cast<const char*>(pkt->data.frame.buf),
                             static_cast<int>(pkt->data.frame.sz));
            break;
        }
    }

    return !outPacket.isEmpty();
}

bool VpxVideoPipeline::encodeImage(const QImage& image, QByteArray& outPacket, bool forceKeyframe) {
    if (image.isNull()) return false;

    QImage scaled = image;
    if (image.width() != m_width || image.height() != m_height || image.format() != QImage::Format_RGB32) {
        scaled = image.scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::FastTransformation)
                      .convertToFormat(QImage::Format_RGB32);
    }

    // Convert RGB32 to YUV420P
    int w = m_width;
    int h = m_height;
    QVector<quint8> yBuf(w * h);
    QVector<quint8> uBuf((w / 2) * (h / 2));
    QVector<quint8> vBuf((w / 2) * (h / 2));

    const quint32* rgbData = reinterpret_cast<const quint32*>(scaled.constBits());
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            quint32 pixel = rgbData[j * w + i];
            int r = (pixel >> 16) & 0xFF;
            int g = (pixel >> 8) & 0xFF;
            int b = pixel & 0xFF;

            // Rec.601 coefficients
            int y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            yBuf[j * w + i] = static_cast<quint8>(std::clamp(y, 0, 255));

            if ((j % 2 == 0) && (i % 2 == 0)) {
                int u = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
                int v = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
                uBuf[(j / 2) * (w / 2) + (i / 2)] = static_cast<quint8>(std::clamp(u, 0, 255));
                vBuf[(j / 2) * (w / 2) + (i / 2)] = static_cast<quint8>(std::clamp(v, 0, 255));
            }
        }
    }

    return encodeYuvFrame(yBuf.constData(), uBuf.constData(), vBuf.constData(), w, w / 2, outPacket, forceKeyframe);
}

bool VpxVideoPipeline::decodePacket(const QByteArray& packet, QImage& outImage) {
    if (!m_decoderInitialized || packet.isEmpty()) return false;

    vpx_codec_err_t res = vpx_codec_decode(&m_decoderCtx,
                                           reinterpret_cast<const uint8_t*>(packet.constData()),
                                           packet.size(),
                                           nullptr, 0);
    if (res != VPX_CODEC_OK) {
        qWarning() << "vpx_codec_decode failed:" << vpx_codec_err_to_string(res);
        return false;
    }

    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* img = vpx_codec_get_frame(&m_decoderCtx, &iter);
    if (!img) return false;

    int w = img->d_w;
    int h = img->d_h;

    outImage = QImage(w, h, QImage::Format_RGB32);
    quint32* dst = reinterpret_cast<quint32*>(outImage.bits());

    // Convert YUV420P to RGB32
    const quint8* yPlane = img->planes[VPX_PLANE_Y];
    const quint8* uPlane = img->planes[VPX_PLANE_U];
    const quint8* vPlane = img->planes[VPX_PLANE_V];
    int yStride = img->stride[VPX_PLANE_Y];
    int uStride = img->stride[VPX_PLANE_U];
    int vStride = img->stride[VPX_PLANE_V];

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            int yVal = yPlane[j * yStride + i] - 16;
            int uVal = uPlane[(j / 2) * uStride + (i / 2)] - 128;
            int vVal = vPlane[(j / 2) * vStride + (i / 2)] - 128;

            int c = yVal * 298;
            int r = std::clamp((c + 409 * vVal + 128) >> 8, 0, 255);
            int g = std::clamp((c - 100 * uVal - 208 * vVal + 128) >> 8, 0, 255);
            int b = std::clamp((c + 516 * uVal + 128) >> 8, 0, 255);

            dst[j * w + i] = 0xFF000000 | (r << 16) | (g << 8) | b;
        }
    }

    return true;
}

} // namespace Onyx

#ifndef VPX_VIDEO_PIPELINE_H
#define VPX_VIDEO_PIPELINE_H

#include <QImage>
#include <QByteArray>
#include <QVector>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include "../core/NetworkConfig.h"

namespace Onyx {

class VpxVideoPipeline {
public:
    enum VpxCodec {
        VP8 = 0,
        VP9 = 1,
        CodecVP8 = 0,
        CodecVP9 = 1
    };

    VpxVideoPipeline(VpxCodec codec = VP8, int width = DEFAULT_VIDEO_WIDTH, int height = DEFAULT_VIDEO_HEIGHT, int fps = DEFAULT_VIDEO_FPS);
    ~VpxVideoPipeline();

    bool initialize();
    void cleanup();

    // Encodes a YUV420P frame into a VP8/VP9 packet
    bool encodeYuvFrame(const quint8* yPlane, const quint8* uPlane, const quint8* vPlane,
                        int yStride, int uvStride, QByteArray& outPacket, bool forceKeyframe = false);

    // Encodes a QImage (RGB32/ARGB32) into a VP8/VP9 packet
    bool encodeImage(const QImage& image, QByteArray& outPacket, bool forceKeyframe = false);

    // Decodes a VP8/VP9 packet back into a QImage
    bool decodePacket(const QByteArray& packet, QImage& outImage);

    void setBitrate(int kbps);
    VpxCodec codec() const { return m_codec; }
    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    VpxCodec m_codec;
    int m_width;
    int m_height;
    int m_fps;
    int m_bitrateKbps = 800; // 800 kbps target video bitrate
    int m_frameCounter = 0;

    vpx_codec_ctx_t m_encoderCtx;
    vpx_codec_ctx_t m_decoderCtx;
    vpx_image_t* m_rawImage = nullptr;

    bool m_encoderInitialized = false;
    bool m_decoderInitialized = false;
};

} // namespace Onyx

#endif // VPX_VIDEO_PIPELINE_H

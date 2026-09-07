#include "FFmpegProcessor.h"
#include <QDebug>
#include <QFile>

namespace Onyx {

FFmpegProcessor::FFmpegProcessor() {
}

FFmpegProcessor::~FFmpegProcessor() {
    stopRecording();
}

bool FFmpegProcessor::convertRgb32ToYuv420(const QImage& srcImage, int targetWidth, int targetHeight,
                                          QByteArray& outY, QByteArray& outU, QByteArray& outV,
                                          int& outYStride, int& outUvStride) {
    if (srcImage.isNull() || targetWidth <= 0 || targetHeight <= 0) return false;

    QImage img = srcImage.format() == QImage::Format_RGB32 ? srcImage : srcImage.convertToFormat(QImage::Format_RGB32);

    SwsContext* sws = sws_getContext(img.width(), img.height(), AV_PIX_FMT_BGRA,
                                     targetWidth, targetHeight, AV_PIX_FMT_YUV420P,
                                     SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws) {
        qWarning() << "Failed to create SwsContext for RGB32 -> YUV420P";
        return false;
    }

    outYStride = targetWidth;
    outUvStride = targetWidth / 2;

    outY.resize(outYStride * targetHeight);
    outU.resize(outUvStride * (targetHeight / 2));
    outV.resize(outUvStride * (targetHeight / 2));

    const uint8_t* srcSlice[4] = { reinterpret_cast<const uint8_t*>(img.constBits()), nullptr, nullptr, nullptr };
    int srcStride[4] = { static_cast<int>(img.bytesPerLine()), 0, 0, 0 };

    uint8_t* dstSlice[4] = {
        reinterpret_cast<uint8_t*>(outY.data()),
        reinterpret_cast<uint8_t*>(outU.data()),
        reinterpret_cast<uint8_t*>(outV.data()),
        nullptr
    };
    int dstStride[4] = { outYStride, outUvStride, outUvStride, 0 };

    sws_scale(sws, srcSlice, srcStride, 0, img.height(), dstSlice, dstStride);
    sws_freeContext(sws);
    return true;
}

QImage FFmpegProcessor::convertYuv420ToRgb32(const quint8* yPlane, const quint8* uPlane, const quint8* vPlane,
                                            int yStride, int uvStride, int width, int height) {
    if (!yPlane || !uPlane || !vPlane || width <= 0 || height <= 0) return QImage();

    QImage out(width, height, QImage::Format_RGB32);

    SwsContext* sws = sws_getContext(width, height, AV_PIX_FMT_YUV420P,
                                     width, height, AV_PIX_FMT_BGRA,
                                     SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws) return QImage();

    const uint8_t* srcSlice[4] = { yPlane, uPlane, vPlane, nullptr };
    int srcStride[4] = { yStride, uvStride, uvStride, 0 };

    uint8_t* dstSlice[4] = { reinterpret_cast<uint8_t*>(out.bits()), nullptr, nullptr, nullptr };
    int dstStride[4] = { static_cast<int>(out.bytesPerLine()), 0, 0, 0 };

    sws_scale(sws, srcSlice, srcStride, 0, height, dstSlice, dstStride);
    sws_freeContext(sws);
    return out;
}

bool FFmpegProcessor::startRecording(const QString& outputPath, int width, int height, int fps) {
    stopRecording();

    m_outputPath = outputPath;
    m_recordWidth = width;
    m_recordHeight = height;
    m_recordFps = fps;
    m_frameIndex = 0;

    // Use AVI container for raw/fast recording
    int ret = avformat_alloc_output_context2(&m_formatCtx, nullptr, "avi", outputPath.toUtf8().constData());
    if (ret < 0 || !m_formatCtx) {
        qWarning() << "Failed to allocate format context for recording:" << outputPath;
        return false;
    }

    m_videoStream = avformat_new_stream(m_formatCtx, nullptr);
    if (!m_videoStream) {
        qWarning() << "Failed to create video stream for recording";
        stopRecording();
        return false;
    }

    m_videoStream->id = 0;
    m_videoStream->time_base = { 1, m_recordFps };

    AVCodecParameters* par = m_videoStream->codecpar;
    par->codec_type = AVMEDIA_TYPE_VIDEO;
    par->codec_id = AV_CODEC_ID_RAWVIDEO;
    par->format = AV_PIX_FMT_BGR24;
    par->width = m_recordWidth;
    par->height = m_recordHeight;

    if (!(m_formatCtx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&m_formatCtx->pb, outputPath.toUtf8().constData(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            qWarning() << "Could not open recording output file:" << outputPath;
            stopRecording();
            return false;
        }
    }

    ret = avformat_write_header(m_formatCtx, nullptr);
    if (ret < 0) {
        qWarning() << "Error occurred when writing recording header";
        stopRecording();
        return false;
    }

    m_swsRecordCtx = sws_getContext(m_recordWidth, m_recordHeight, AV_PIX_FMT_BGRA,
                                    m_recordWidth, m_recordHeight, AV_PIX_FMT_BGR24,
                                    SWS_POINT, nullptr, nullptr, nullptr);

    m_recordFrame = av_frame_alloc();
    m_recordFrame->format = AV_PIX_FMT_BGR24;
    m_recordFrame->width = m_recordWidth;
    m_recordFrame->height = m_recordHeight;
    av_frame_get_buffer(m_recordFrame, 0);

    m_recordPkt = av_packet_alloc();

    m_recording = true;
    return true;
}

bool FFmpegProcessor::recordFrame(const QImage& frame) {
    if (!m_recording || !m_formatCtx || !m_recordFrame || !m_swsRecordCtx) return false;

    QImage scaled = frame;
    if (frame.width() != m_recordWidth || frame.height() != m_recordHeight || frame.format() != QImage::Format_RGB32) {
        scaled = frame.scaled(m_recordWidth, m_recordHeight, Qt::IgnoreAspectRatio, Qt::FastTransformation)
                      .convertToFormat(QImage::Format_RGB32);
    }

    const uint8_t* srcSlice[4] = { reinterpret_cast<const uint8_t*>(scaled.constBits()), nullptr, nullptr, nullptr };
    int srcStride[4] = { static_cast<int>(scaled.bytesPerLine()), 0, 0, 0 };

    sws_scale(m_swsRecordCtx, srcSlice, srcStride, 0, m_recordHeight,
              m_recordFrame->data, m_recordFrame->linesize);

    m_recordFrame->pts = m_frameIndex++;

    av_packet_unref(m_recordPkt);
    m_recordPkt->data = m_recordFrame->data[0];
    m_recordPkt->size = m_recordFrame->linesize[0] * m_recordHeight;
    m_recordPkt->pts = m_recordFrame->pts;
    m_recordPkt->dts = m_recordFrame->pts;
    m_recordPkt->stream_index = m_videoStream->index;

    int ret = av_interleaved_write_frame(m_formatCtx, m_recordPkt);
    return ret >= 0;
}

void FFmpegProcessor::stopRecording() {
    if (m_formatCtx) {
        if (m_recording) {
            av_write_trailer(m_formatCtx);
        }
        if (!(m_formatCtx->oformat->flags & AVFMT_NOFILE) && m_formatCtx->pb) {
            avio_closep(&m_formatCtx->pb);
        }
        avformat_free_context(m_formatCtx);
        m_formatCtx = nullptr;
    }

    if (m_swsRecordCtx) {
        sws_freeContext(m_swsRecordCtx);
        m_swsRecordCtx = nullptr;
    }
    if (m_recordFrame) {
        av_frame_free(&m_recordFrame);
        m_recordFrame = nullptr;
    }
    if (m_recordPkt) {
        av_packet_free(&m_recordPkt);
        m_recordPkt = nullptr;
    }

    m_recording = false;
    m_videoStream = nullptr;
}

} // namespace Onyx

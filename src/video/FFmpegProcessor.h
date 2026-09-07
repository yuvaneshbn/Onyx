#ifndef FFMPEG_PROCESSOR_H
#define FFMPEG_PROCESSOR_H

#include <QImage>
#include <QByteArray>
#include <QString>

extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

namespace Onyx {

class FFmpegProcessor {
public:
    FFmpegProcessor();
    ~FFmpegProcessor();

    // High-performance color conversion using sws_scale
    static bool convertRgb32ToYuv420(const QImage& srcImage, int targetWidth, int targetHeight,
                                     QByteArray& outY, QByteArray& outU, QByteArray& outV,
                                     int& outYStride, int& outUvStride);

    static QImage convertYuv420ToRgb32(const quint8* yPlane, const quint8* uPlane, const quint8* vPlane,
                                       int yStride, int uvStride, int width, int height);

    // Live session recording to disk
    bool startRecording(const QString& outputPath, int width, int height, int fps = 30);
    bool recordFrame(const QImage& frame);
    void stopRecording();

    bool isRecording() const { return m_recording; }
    QString recordingPath() const { return m_outputPath; }

private:
    bool m_recording = false;
    QString m_outputPath;
    int m_recordWidth = 640;
    int m_recordHeight = 480;
    int m_recordFps = 30;
    int64_t m_frameIndex = 0;

    AVFormatContext* m_formatCtx = nullptr;
    AVStream* m_videoStream = nullptr;
    SwsContext* m_swsRecordCtx = nullptr;
    AVFrame* m_recordFrame = nullptr;
    AVPacket* m_recordPkt = nullptr;
};

} // namespace Onyx

#endif // FFMPEG_PROCESSOR_H

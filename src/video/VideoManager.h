#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QImage>
#include <QMap>
#include "VpxVideoPipeline.h"
#include "FFmpegProcessor.h"
#include "../filetransfer/CryptoEngine.h"
#include "../core/NetworkConfig.h"

namespace Onyx {

class VideoManager : public QObject {
    Q_OBJECT
public:
    explicit VideoManager(QObject* parent = nullptr);
    ~VideoManager() override;

    bool startCamera();
    void stopCamera();

    void setVideoCodec(VpxVideoPipeline::VpxCodec codec);
    VpxVideoPipeline::VpxCodec videoCodec() const { return m_codec; }

    void setResolution(int width, int height);
    QSize resolution() const { return QSize(m_width, m_height); }

    void setFrameRate(int fps);
    int frameRate() const { return m_fps; }

    void setBitrate(int kbps);

    void setEncryptionKey(const QByteArray& key) { m_encryptionKey = key; }
    void setLocalPeerId(const QString& peerId) { m_localPeerId = peerId; }

    void setTargetPeer(const QString& host, quint16 rtpPort);
    void clearTargetPeer();
    void setGroupCallMode(bool enabled);

    bool bindLocalMediaPort(quint16 port = GROUP_VIDEO_PORT);

    bool startRecording(const QString& filePath);
    void stopRecording();
    bool isRecording() const { return m_ffmpegProcessor.isRecording(); }

    bool isCameraRunning() const { return m_cameraRunning; }

signals:
    void localFrameCaptured(const QImage& frame);
    void remoteFrameReceived(const QImage& frame);
    void peerVideoFrameReceived(const QString& peerId, const QImage& frame);
    void videoError(const QString& errorMsg);

private slots:
    void onFrameTimer();
    void onIncomingUdpDatagram();

private:
    void processLocalFrame(const QImage& image);
    void sendFramePackets(const QByteArray& framePayload);

    QTimer* m_frameTimer = nullptr;
    int m_simulatedAngle = 0;
    bool m_cameraRunning = false;

    int m_width = DEFAULT_VIDEO_WIDTH;
    int m_height = DEFAULT_VIDEO_HEIGHT;
    int m_fps = DEFAULT_VIDEO_FPS;
    int m_bitrateKbps = 1500;
    VpxVideoPipeline::VpxCodec m_codec = VpxVideoPipeline::CodecVP8;

    QUdpSocket* m_udpSocket = nullptr;
    QHostAddress m_targetAddress;
    quint16 m_targetPort = 0;
    bool m_groupMode = true;

    VpxVideoPipeline m_localEncoder;
    FFmpegProcessor m_ffmpegProcessor;

    // Per-peer decoder pipelines
    QMap<QString, VpxVideoPipeline*> m_peerDecoders;

    // Chunk reassembly buffer
    struct FrameReassembly {
        quint16 frameId = 0;
        quint8 totalChunks = 0;
        QMap<quint8, QByteArray> chunks;
        qint64 lastUpdateMs = 0;
    };
    QMap<QString, FrameReassembly> m_reassemblyMap;

    quint16 m_frameSeq = 0;
    QByteArray m_encryptionKey;
    QString m_localPeerId = "local_peer";
};

} // namespace Onyx

#endif // VIDEO_MANAGER_H

#include "VideoManager.h"
#include <QPainter>
#include <QDateTime>
#include <cmath>
#include <QDebug>

namespace Onyx {

VideoManager::VideoManager(QObject* parent)
    : QObject(parent),
      m_localEncoder(VpxVideoPipeline::CodecVP8, DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_FPS) {
    m_localEncoder.initialize();
}

VideoManager::~VideoManager() {
    stopCamera();
    stopRecording();
    for (auto* dec : m_peerDecoders) {
        delete dec;
    }
    m_peerDecoders.clear();

    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }
}

void VideoManager::setResolution(int width, int height) {
    if (m_width == width && m_height == height) return;
    m_width = width;
    m_height = height;
    m_localEncoder = VpxVideoPipeline(m_codec, m_width, m_height, m_fps);
    m_localEncoder.initialize();
}

void VideoManager::setFrameRate(int fps) {
    if (m_fps == fps) return;
    m_fps = qBound(10, fps, 60);
    if (m_frameTimer && m_cameraRunning) {
        m_frameTimer->setInterval(1000 / m_fps);
    }
    m_localEncoder = VpxVideoPipeline(m_codec, m_width, m_height, m_fps);
    m_localEncoder.initialize();
}

void VideoManager::setBitrate(int kbps) {
    m_bitrateKbps = qBound(250, kbps, 8000);
}

void VideoManager::setVideoCodec(VpxVideoPipeline::VpxCodec codec) {
    m_codec = codec;
    m_localEncoder = VpxVideoPipeline(m_codec, m_width, m_height, m_fps);
    m_localEncoder.initialize();
}

void VideoManager::setGroupCallMode(bool enabled) {
    m_groupMode = enabled;
}

bool VideoManager::bindLocalMediaPort(quint16 port) {
    if (!m_udpSocket) {
        m_udpSocket = new QUdpSocket(this);
        connect(m_udpSocket, &QUdpSocket::readyRead, this, &VideoManager::onIncomingUdpDatagram);
    }

    if (m_udpSocket->state() == QAbstractSocket::BoundState) {
        m_udpSocket->close();
    }

    bool bound = m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (bound) {
        m_udpSocket->joinMulticastGroup(QHostAddress(MULTICAST_GROUP));
    }
    return bound;
}

void VideoManager::setTargetPeer(const QString& host, quint16 rtpPort) {
    m_targetAddress = QHostAddress(host);
    m_targetPort = rtpPort;
}

void VideoManager::clearTargetPeer() {
    m_targetAddress.clear();
    m_targetPort = 0;
}

bool VideoManager::startCamera() {
    stopCamera();

    m_frameTimer = new QTimer(this);
    connect(m_frameTimer, &QTimer::timeout, this, &VideoManager::onFrameTimer);
    m_frameTimer->start(1000 / m_fps);

    m_cameraRunning = true;
    return true;
}

void VideoManager::stopCamera() {
    if (m_frameTimer) {
        m_frameTimer->stop();
        delete m_frameTimer;
        m_frameTimer = nullptr;
    }
    m_cameraRunning = false;
}

void VideoManager::onFrameTimer() {
    // High quality synthesized dynamic video feed with animated grid, avatar and stats
    QImage testFrame(m_width, m_height, QImage::Format_RGB32);
    testFrame.fill(QColor(15, 23, 42));

    QPainter p(&testFrame);
    p.setRenderHint(QPainter::Antialiasing);

    // Glowing orbital circles
    m_simulatedAngle = (m_simulatedAngle + 3) % 360;
    double rad = m_simulatedAngle * 3.14159265 / 180.0;
    int centerX = m_width / 2;
    int centerY = m_height / 2;

    int orbX = static_cast<int>(centerX + (m_width * 0.22) * std::cos(rad));
    int orbY = static_cast<int>(centerY + (m_height * 0.22) * std::sin(rad));

    QRadialGradient grad(orbX, orbY, m_height * 0.25);
    grad.setColorAt(0, QColor(0, 120, 212, 220));
    grad.setColorAt(1, QColor(0, 120, 212, 0));
    p.setBrush(grad);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(orbX, orbY), int(m_height * 0.25), int(m_height * 0.25));

    // Subtle grid
    p.setPen(QColor(30, 41, 59, 120));
    for (int x = 0; x < m_width; x += 40) p.drawLine(x, 0, x, m_height);
    for (int y = 0; y < m_height; y += 40) p.drawLine(0, y, m_width, y);

    // Center participant silhouette / avatar
    int avatarR = m_height * 0.16;
    p.setBrush(QColor(30, 41, 59, 190));
    p.setPen(QPen(QColor(0, 120, 212), 2));
    p.drawEllipse(QPoint(centerX, centerY - 20), avatarR, avatarR);

    // Initial / User icon inside avatar
    p.setPen(QColor(241, 245, 249));
    QFont avFont = p.font();
    avFont.setPointSize(avatarR * 0.6);
    avFont.setBold(true);
    p.setFont(avFont);
    p.drawText(QRect(centerX - avatarR, centerY - 20 - avatarR, avatarR * 2, avatarR * 2),
               Qt::AlignCenter, m_localPeerId.left(1).toUpper());

    // Header badge
    p.setPen(QColor(241, 245, 249));
    QFont font = p.font();
    font.setPointSize(12);
    font.setBold(true);
    p.setFont(font);
    p.drawText(24, 36, "Onyx Live Video (Broadcasting)");

    font.setPointSize(9);
    font.setBold(false);
    p.setFont(font);
    p.setPen(QColor(148, 163, 184));
    p.drawText(24, 60, QString("Codec: %1 | Res: %2x%3 @ %4 FPS | E2EE: %5")
               .arg(m_codec == VpxVideoPipeline::VP8 ? "VP8" : "VP9")
               .arg(m_width).arg(m_height).arg(m_fps)
               .arg(!m_encryptionKey.isEmpty() ? "AES-256-GCM" : "Plain"));

    p.drawText(24, 80, QString("Timestamp: %1")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")));

    p.end();
    processLocalFrame(testFrame);
}

void VideoManager::processLocalFrame(const QImage& image) {
    emit localFrameCaptured(image);

    if (m_ffmpegProcessor.isRecording()) {
        m_ffmpegProcessor.recordFrame(image);
    }

    // Encode via VPX (VP8/VP9)
    QByteArray rawFrame;
    if (m_localEncoder.encodeImage(image, rawFrame)) {
        // Encrypt frame with E2EE
        QByteArray finalPayload = rawFrame;
        if (!m_encryptionKey.isEmpty()) {
            QByteArray enc = CryptoEngine::encryptEnvelope(rawFrame, m_encryptionKey);
            if (!enc.isEmpty()) finalPayload = enc;
        }

        sendFramePackets(finalPayload);
    }
}

void VideoManager::sendFramePackets(const QByteArray& framePayload) {
    if (!m_udpSocket) return;

    m_frameSeq++;
    constexpr int CHUNK_PAYLOAD_SIZE = 1200; // Safe for LAN UDP MTU (1500)
    int totalChunks = (framePayload.size() + CHUNK_PAYLOAD_SIZE - 1) / CHUNK_PAYLOAD_SIZE;
    if (totalChunks > 255) return; // Limit frame size

    char codecTag = (m_codec == VpxVideoPipeline::VP8) ? '8' : '9';
    QByteArray idBytes = m_localPeerId.toUtf8();
    quint8 idLen = static_cast<quint8>(qMin(idBytes.size(), 64));

    for (quint8 chunkIdx = 0; chunkIdx < totalChunks; ++chunkIdx) {
        int offset = chunkIdx * CHUNK_PAYLOAD_SIZE;
        int len = qMin(CHUNK_PAYLOAD_SIZE, framePayload.size() - offset);

        QByteArray packet;
        packet.reserve(10 + idLen + len);
        packet.append("ONYV", 4);
        packet.append(codecTag);
        packet.append(static_cast<char>(idLen));
        packet.append(idBytes.constData(), idLen);
        
        // 2-byte Frame Seq
        quint16 netSeq = qToBigEndian(m_frameSeq);
        packet.append(reinterpret_cast<const char*>(&netSeq), 2);
        packet.append(static_cast<char>(chunkIdx));
        packet.append(static_cast<char>(totalChunks));
        packet.append(framePayload.constData() + offset, len);

        if (m_groupMode) {
            m_udpSocket->writeDatagram(packet, QHostAddress(MULTICAST_GROUP), GROUP_VIDEO_PORT);
        } else if (!m_targetAddress.isNull() && m_targetPort > 0) {
            m_udpSocket->writeDatagram(packet, m_targetAddress, m_targetPort);
        }
    }
}

void VideoManager::onIncomingUdpDatagram() {
    while (m_udpSocket && m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 port;
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &port);

        if (datagram.size() < 10) continue;
        if (datagram.left(4) != "ONYV") continue;

        char codecTag = datagram.at(4);
        quint8 idLen = static_cast<quint8>(datagram.at(5));
        if (datagram.size() < 10 + idLen) continue;

        QString peerId = QString::fromUtf8(datagram.mid(6, idLen));
        if (peerId == m_localPeerId) continue; // Skip local loopback

        int headerPos = 6 + idLen;
        quint16 frameId = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(datagram.constData() + headerPos));
        quint8 chunkIdx = static_cast<quint8>(datagram.at(headerPos + 2));
        quint8 totalChunks = static_cast<quint8>(datagram.at(headerPos + 3));
        QByteArray chunkData = datagram.mid(headerPos + 4);

        auto& reasm = m_reassemblyMap[peerId];
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        if (reasm.frameId != frameId) {
            reasm.frameId = frameId;
            reasm.totalChunks = totalChunks;
            reasm.chunks.clear();
            reasm.lastUpdateMs = now;
        }

        reasm.chunks[chunkIdx] = chunkData;

        // Check if all chunks received
        if (reasm.chunks.size() == totalChunks) {
            QByteArray completePayload;
            for (quint8 i = 0; i < totalChunks; ++i) {
                completePayload.append(reasm.chunks.value(i));
            }
            reasm.chunks.clear();

            // Decrypt if encrypted
            QByteArray plainPayload;
            if (!m_encryptionKey.isEmpty()) {
                if (!CryptoEngine::decryptEnvelope(completePayload, m_encryptionKey, plainPayload)) {
                    plainPayload = completePayload;
                }
            } else {
                plainPayload = completePayload;
            }

            // Decode frame via peer-specific VPX decoder
            if (!m_peerDecoders.contains(peerId)) {
                VpxVideoPipeline::VpxCodec c = (codecTag == '9') ? VpxVideoPipeline::CodecVP9 : VpxVideoPipeline::CodecVP8;
                VpxVideoPipeline* dec = new VpxVideoPipeline(c, m_width, m_height, m_fps);
                dec->initialize();
                m_peerDecoders.insert(peerId, dec);
            }

            QImage remoteImage;
            if (m_peerDecoders[peerId]->decodePacket(plainPayload, remoteImage)) {
                emit remoteFrameReceived(remoteImage);
                emit peerVideoFrameReceived(peerId, remoteImage);

                if (m_ffmpegProcessor.isRecording()) {
                    m_ffmpegProcessor.recordFrame(remoteImage);
                }
            }
        }
    }
}

bool VideoManager::startRecording(const QString& filePath) {
    return m_ffmpegProcessor.startRecording(filePath, m_width, m_height, m_fps);
}

void VideoManager::stopRecording() {
    m_ffmpegProcessor.stopRecording();
}

} // namespace Onyx

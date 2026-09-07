#include "VideoGridWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolTip>

namespace Onyx {

VideoGridWidget::VideoGridWidget(QWidget* parent)
    : QWidget(parent) {
    setAttribute(Qt::WA_OpaquePaintEvent);
    setupFloatingControls();
}

void VideoGridWidget::setupFloatingControls() {
    m_controlsContainer = new QWidget(this);
    m_controlsContainer->setObjectName("floatingControlsBar");
    m_controlsContainer->setStyleSheet(
        "#floatingControlsBar {"
        "  background-color: rgba(15, 23, 42, 0.95);"
        "  border: 1px solid #334155;"
        "  border-radius: 29px;"
        "}"
        "#floatingControlsBar QPushButton {"
        "  background-color: #1E293B;"
        "  border: 1px solid #334155;"
        "  border-radius: 20px;"
        "  min-width: 40px;"
        "  max-width: 40px;"
        "  min-height: 40px;"
        "  max-height: 40px;"
        "  padding: 0px;"
        "}"
        "#floatingControlsBar QPushButton:hover {"
        "  background-color: #334155;"
        "  border: 1px solid #0078D4;"
        "}"
        "#floatingControlsBar QPushButton#endCallBtn {"
        "  background-color: #EF4444;"
        "  border: none;"
        "  min-width: 46px;"
        "  max-width: 46px;"
        "  min-height: 46px;"
        "  max-height: 46px;"
        "  border-radius: 23px;"
        "}"
        "#floatingControlsBar QPushButton#endCallBtn:hover {"
        "  background-color: #DC2626;"
        "}"
        "#floatingControlsBar QPushButton:checked {"
        "  background-color: #0078D4;"
        "  border: 1px solid #38BDF8;"
        "}"
    );

    QHBoxLayout* layout = new QHBoxLayout(m_controlsContainer);
    layout->setContentsMargins(16, 6, 16, 6);
    layout->setSpacing(8);

    m_securityBtn = new QPushButton(m_controlsContainer);
    m_securityBtn->setObjectName("securityBtn");
    m_securityBtn->setIcon(QIcon(":/icons/shield.svg"));
    m_securityBtn->setIconSize(QSize(20, 20));
    m_securityBtn->setToolTip("E2EE Security: AES-256-GCM Active");

    m_shareBtn = new QPushButton(m_controlsContainer);
    m_shareBtn->setIcon(QIcon(":/icons/screen-share.svg"));
    m_shareBtn->setIconSize(QSize(20, 20));
    m_shareBtn->setToolTip("Share Screen");
    connect(m_shareBtn, &QPushButton::clicked, this, &VideoGridWidget::shareScreenRequested);

    m_videoBtn = new QPushButton(m_controlsContainer);
    m_videoBtn->setCheckable(true);
    m_videoBtn->setChecked(true);
    m_videoBtn->setIcon(QIcon(":/icons/video.svg"));
    m_videoBtn->setIconSize(QSize(20, 20));
    m_videoBtn->setToolTip("Toggle Camera");
    connect(m_videoBtn, &QPushButton::clicked, this, &VideoGridWidget::toggleVideoRequested);

    m_micBtn = new QPushButton(m_controlsContainer);
    m_micBtn->setCheckable(true);
    m_micBtn->setChecked(true);
    m_micBtn->setIcon(QIcon(":/icons/mic.svg"));
    m_micBtn->setIconSize(QSize(20, 20));
    m_micBtn->setToolTip("Toggle Microphone");
    connect(m_micBtn, &QPushButton::clicked, this, &VideoGridWidget::toggleMicRequested);

    m_endCallBtn = new QPushButton(m_controlsContainer);
    m_endCallBtn->setObjectName("endCallBtn");
    m_endCallBtn->setIcon(QIcon(":/icons/call-end.svg"));
    m_endCallBtn->setIconSize(QSize(24, 24));
    m_endCallBtn->setToolTip("End Call");
    connect(m_endCallBtn, &QPushButton::clicked, this, &VideoGridWidget::endCallRequested);

    m_chatBtn = new QPushButton(m_controlsContainer);
    m_chatBtn->setIcon(QIcon(":/icons/chat.svg"));
    m_chatBtn->setIconSize(QSize(20, 20));
    m_chatBtn->setToolTip("Toggle In-Call Chat");
    connect(m_chatBtn, &QPushButton::clicked, this, &VideoGridWidget::toggleChatRequested);

    m_recordBtn = new QPushButton(m_controlsContainer);
    m_recordBtn->setIcon(QIcon(":/icons/record.svg"));
    m_recordBtn->setIconSize(QSize(20, 20));
    m_recordBtn->setToolTip("Record Call Session");
    connect(m_recordBtn, &QPushButton::clicked, this, [this]() {
        m_isRecording = !m_isRecording;
        m_recordBtn->setStyleSheet(m_isRecording ? "background-color: #EF4444; border: 1px solid #F87171;" : "");
        emit toggleRecordingRequested();
    });

    m_participantsBtn = new QPushButton(m_controlsContainer);
    m_participantsBtn->setIcon(QIcon(":/icons/users.svg"));
    m_participantsBtn->setIconSize(QSize(20, 20));
    m_participantsBtn->setToolTip("Participant List");

    m_settingsBtn = new QPushButton(m_controlsContainer);
    m_settingsBtn->setIcon(QIcon(":/icons/settings.svg"));
    m_settingsBtn->setIconSize(QSize(20, 20));
    m_settingsBtn->setToolTip("Call Settings");
    connect(m_settingsBtn, &QPushButton::clicked, this, &VideoGridWidget::openSettingsRequested);

    layout->addWidget(m_securityBtn);
    layout->addWidget(m_shareBtn);
    layout->addWidget(m_videoBtn);
    layout->addWidget(m_micBtn);
    layout->addWidget(m_endCallBtn);
    layout->addWidget(m_chatBtn);
    layout->addWidget(m_recordBtn);
    layout->addWidget(m_participantsBtn);
    layout->addWidget(m_settingsBtn);
}

void VideoGridWidget::setLocalPeer(const QString& peerId, const QString& name, const QString& avatarColor) {
    m_localPeerId = peerId;
    m_localName = name;
    m_localAvatarColor = avatarColor;
    update();
}

void VideoGridWidget::updateLocalFrame(const QImage& frame) {
    m_localFrame = frame;
    update();
}

void VideoGridWidget::updatePeerFrame(const QString& peerId, const QImage& frame) {
    if (m_participants.contains(peerId)) {
        m_participants[peerId].lastFrame = frame;
        update();
    }
}

void VideoGridWidget::updatePeerSpeaking(const QString& peerId, bool isSpeaking) {
    if (m_participants.contains(peerId)) {
        m_participants[peerId].isSpeaking = isSpeaking;
        update();
    }
}

void VideoGridWidget::updatePeerMediaState(const QString& peerId, bool micMuted, bool videoMuted) {
    if (m_participants.contains(peerId)) {
        m_participants[peerId].micMuted = micMuted;
        m_participants[peerId].videoMuted = videoMuted;
        update();
    }
}

void VideoGridWidget::addParticipant(const QString& peerId, const QString& name, const QString& avatarColor) {
    ParticipantTile tile;
    tile.peerId = peerId;
    tile.name = name;
    tile.avatarColor = avatarColor;
    m_participants.insert(peerId, tile);
    m_participantsBtn->setToolTip(QString("Participants (%1)").arg(m_participants.size() + 1));
    update();
}

void VideoGridWidget::removeParticipant(const QString& peerId) {
    m_participants.remove(peerId);
    m_participantsBtn->setToolTip(QString("Participants (%1)").arg(m_participants.size() + 1));
    update();
}

void VideoGridWidget::clearParticipants() {
    m_participants.clear();
    update();
}

void VideoGridWidget::setLocalMicMuted(bool muted) {
    m_localMicMuted = muted;
    m_micBtn->setChecked(!muted);
    m_micBtn->setIcon(QIcon(muted ? ":/icons/mic-off.svg" : ":/icons/mic.svg"));
    m_micBtn->setToolTip(muted ? "Unmute Microphone" : "Mute Microphone");
    update();
}

void VideoGridWidget::setLocalVideoMuted(bool muted) {
    m_localVideoMuted = muted;
    m_videoBtn->setChecked(!muted);
    m_videoBtn->setIcon(QIcon(muted ? ":/icons/video-off.svg" : ":/icons/video.svg"));
    m_videoBtn->setToolTip(muted ? "Turn On Camera" : "Turn Off Camera");
    update();
}

void VideoGridWidget::setCallActive(bool active) {
    m_callActive = active;
    m_controlsContainer->setVisible(active);
    update();
}

void VideoGridWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (m_controlsContainer) {
        int w = 500;
        int h = 58;
        int x = (width() - w) / 2;
        int y = height() - h - 20;
        m_controlsContainer->setGeometry(x, y, w, h);
    }
}

void VideoGridWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Deep Dark Canvas
    p.fillRect(rect(), QColor(10, 15, 26));

    // Combine local + remote participants into tile list
    QList<ParticipantTile> tiles;
    
    // Remote participants first
    for (const auto& tile : m_participants) {
        tiles.append(tile);
    }

    // Local user tile
    ParticipantTile localTile;
    localTile.peerId = m_localPeerId;
    localTile.name = m_localName + " (You)";
    localTile.avatarColor = m_localAvatarColor;
    localTile.lastFrame = m_localFrame;
    localTile.micMuted = m_localMicMuted;
    localTile.videoMuted = m_localVideoMuted;
    tiles.append(localTile);

    int count = tiles.size();
    if (count == 0) {
        p.setPen(QColor(148, 163, 184));
        QFont f = p.font();
        f.setPointSize(14);
        p.setFont(f);
        p.drawText(rect(), Qt::AlignCenter, "No Active Video Session\nStart a Call or Join a Group Conference");
        return;
    }

    // Determine grid rows and columns
    int cols = 1;
    int rows = 1;
    if (count == 2) {
        cols = 2; rows = 1;
    } else if (count <= 4) {
        cols = 2; rows = 2;
    } else if (count <= 6) {
        cols = 3; rows = 2;
    } else {
        cols = 3; rows = 3;
    }

    int margin = 12;
    int bottomReserve = m_callActive ? 90 : 20;
    int availW = width() - (margin * (cols + 1));
    int availH = height() - bottomReserve - (margin * (rows + 1));
    int tileW = availW / cols;
    int tileH = availH / rows;

    for (int i = 0; i < count; ++i) {
        int r = i / cols;
        int c = i % cols;
        int x = margin + c * (tileW + margin);
        int y = margin + r * (tileH + margin);
        QRect tileRect(x, y, tileW, tileH);

        const auto& t = tiles[i];

        // Rounded Tile Card
        QPainterPath path;
        path.addRoundedRect(tileRect, 12, 12);
        p.save();
        p.setClipPath(path);

        // Tile background
        p.fillRect(tileRect, QColor(24, 32, 47));

        // Draw Video or Avatar
        if (!t.videoMuted && !t.lastFrame.isNull()) {
            QImage scaled = t.lastFrame.scaled(tileRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            int sx = tileRect.x() + (tileRect.width() - scaled.width()) / 2;
            int sy = tileRect.y() + (tileRect.height() - scaled.height()) / 2;
            p.drawImage(sx, sy, scaled);
        } else {
            // Avatar Circle
            int rR = qMin(tileRect.width(), tileRect.height()) * 0.22;
            QPoint center = tileRect.center();
            center.setY(center.y() - 10);

            p.setBrush(QColor(t.avatarColor));
            p.setPen(Qt::NoPen);
            p.drawEllipse(center, rR, rR);

            // Initial
            p.setPen(Qt::white);
            QFont avFont = p.font();
            avFont.setPointSize(rR * 0.7);
            avFont.setBold(true);
            p.setFont(avFont);
            QString initial = t.name.isEmpty() ? "U" : t.name.left(1).toUpper();
            p.drawText(QRect(center.x() - rR, center.y() - rR, rR * 2, rR * 2), Qt::AlignCenter, initial);
        }

        p.restore();

        // Active Speaker Glowing Green Border
        if (t.isSpeaking) {
            p.save();
            p.setPen(QPen(QColor(16, 185, 129), 3)); // Green border
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(tileRect.adjusted(1, 1, -1, -1), 12, 12);
            p.restore();
        } else {
            p.save();
            p.setPen(QPen(QColor(51, 65, 85, 140), 1));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(tileRect, 12, 12);
            p.restore();
        }

        // Participant Name Tag (Bottom Left Pill)
        int tagH = 24;
        int tagW = qMin(180, tileRect.width() - 20);
        QRect tagRect(tileRect.x() + 10, tileRect.bottom() - tagH - 10, tagW, tagH);

        p.save();
        p.setBrush(QColor(15, 23, 42, 200));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(tagRect, 12, 12);

        p.setPen(QColor(241, 245, 249));
        QFont tagFont = p.font();
        tagFont.setPointSize(9);
        tagFont.setBold(true);
        p.setFont(tagFont);
        QString displayName = t.name;
        if (t.micMuted) displayName += " (Muted)";
        p.drawText(tagRect.adjusted(10, 0, -10, 0), Qt::AlignVCenter | Qt::AlignLeft, displayName);
        p.restore();
    }
}

} // namespace Onyx

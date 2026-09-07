#ifndef VIDEO_GRID_WIDGET_H
#define VIDEO_GRID_WIDGET_H

#include <QWidget>
#include <QImage>
#include <QMap>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

namespace Onyx {

struct ParticipantTile {
    QString peerId;
    QString name;
    QString avatarColor = "#0078D4";
    QImage lastFrame;
    bool isSpeaking = false;
    bool micMuted = false;
    bool videoMuted = false;
};

class VideoGridWidget : public QWidget {
    Q_OBJECT
public:
    explicit VideoGridWidget(QWidget* parent = nullptr);

    void setLocalPeer(const QString& peerId, const QString& name, const QString& avatarColor);
    void updateLocalFrame(const QImage& frame);
    void updatePeerFrame(const QString& peerId, const QImage& frame);
    void updatePeerSpeaking(const QString& peerId, bool isSpeaking);
    void updatePeerMediaState(const QString& peerId, bool micMuted, bool videoMuted);
    void addParticipant(const QString& peerId, const QString& name, const QString& avatarColor = "#0078D4");
    void removeParticipant(const QString& peerId);
    void clearParticipants();

    void setLocalMicMuted(bool muted);
    void setLocalVideoMuted(bool muted);
    void setCallActive(bool active);

signals:
    void toggleMicRequested();
    void toggleVideoRequested();
    void endCallRequested();
    void toggleChatRequested();
    void openSettingsRequested();
    void shareScreenRequested();
    void toggleRecordingRequested();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupFloatingControls();
    void updateLayout();

    QString m_localPeerId = "local";
    QString m_localName = "You";
    QString m_localAvatarColor = "#0078D4";
    QImage m_localFrame;
    bool m_localMicMuted = false;
    bool m_localVideoMuted = false;
    bool m_callActive = false;
    bool m_isRecording = false;

    QMap<QString, ParticipantTile> m_participants;

    // Floating call controls
    QWidget* m_controlsContainer = nullptr;
    QPushButton* m_securityBtn = nullptr;
    QPushButton* m_shareBtn = nullptr;
    QPushButton* m_videoBtn = nullptr;
    QPushButton* m_micBtn = nullptr;
    QPushButton* m_endCallBtn = nullptr;
    QPushButton* m_chatBtn = nullptr;
    QPushButton* m_recordBtn = nullptr;
    QPushButton* m_participantsBtn = nullptr;
    QPushButton* m_settingsBtn = nullptr;
};

} // namespace Onyx

#endif // VIDEO_GRID_WIDGET_H

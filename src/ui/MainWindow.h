#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTimer>
#include <QUdpSocket>
#include <QSplitter>

#include "../core/P2PDiscovery.h"
#include "../core/JsonProtocol.h"
#include "../audio/AudioManager.h"
#include "../video/VideoManager.h"
#include "../voip/PjSipManager.h"
#include "../filetransfer/SecureFileTransferManager.h"
#include "../hardware/HidHeadsetManager.h"
#include "VideoGridWidget.h"
#include "SettingsDialog.h"
#include "LoginDialog.h"

namespace Onyx {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void initializeProfile(const QString& username, const QString& statusMessage, const QString& avatarColor);

private slots:
    // Navigation Rail slots
    void onNavTabChanged(int index);
    void onOpenSettings();
    void onOpenProfile();

    // P2P Discovery & Peer slots
    void onPeerDiscovered(const PeerInfo& peer);
    void onPeerUpdated(const PeerInfo& peer);
    void onPeerLost(const QString& peerId);
    void onPeerSelectionChanged();
    void onSearchFilterChanged(const QString& text);

    // Call Signaling & Group Conference slots
    void onCallSignalReceived(const CallSignal& signal, const QString& senderIp);
    void onStartDirectCall(bool video);
    void onJoinGroupCall();
    void onLeaveCall();
    void onToggleMic();
    void onToggleCamera();
    void onShareScreen();
    void onToggleRecording();
    void onCallTimerTick();

    // Audio / Video stream feedback slots
    void onLocalFrameCaptured(const QImage& frame);
    void onPeerVideoFrameReceived(const QString& peerId, const QImage& frame);
    void onMicLevelChanged(float level);
    void onLocalSpeakingChanged(bool isSpeaking);
    void onPeerSpeakingChanged(const QString& peerId, bool isSpeaking);

    // Chat slots
    void onSendMessage();
    void onSendFileAttachment();
    void onIncomingChatDatagram();

    // File Transfer slots
    void onSendFileDirect();
    void onTransferStarted(const QString& transferId, const QString& fileName, qint64 totalBytes, bool isSending);
    void onTransferProgress(const QString& transferId, qint64 bytesTransferred, qint64 totalBytes, double speedMBs);
    void onTransferCompleted(const QString& transferId, const QString& fileName, bool verifiedSha256);
    void onTransferFailed(const QString& transferId, const QString& reason);

    // Hardware & Headset slots
    void onHeadsetConnected(const HidDeviceInfo& dev);
    void onHeadsetDisconnected();
    void onHeadsetMutePressed();
    void onHeadsetHookSwitchPressed();

    // Settings Applied slot
    void onSettingsApplied();

private:
    void setupUi();
    void setupConnections();
    QWidget* createNavRail();
    QWidget* createSidebar();
    QWidget* createChatView();
    QWidget* createConferenceView();
    QWidget* createFileTransferView();
    QWidget* createContactProfileDrawer();

    void updatePeersListWidget();
    void appendChatMessage(const QString& sender, const QString& content, bool isSelf,
                           const QString& attachmentName = "", qint64 attachmentSize = 0);
    PeerInfo selectedPeer() const;
    void applySecurityKey(const QString& passkey);

    // Core Engines
    PeerInfo m_localPeer;
    P2PDiscovery* m_discovery = nullptr;
    AudioManager* m_audioManager = nullptr;
    VideoManager* m_videoManager = nullptr;
    PjSipManager* m_sipManager = nullptr;
    SecureFileTransferManager* m_fileManager = nullptr;
    HidHeadsetManager* m_hidManager = nullptr;
    QUdpSocket* m_chatSocket = nullptr;
    SettingsDialog* m_settingsDialog = nullptr;

    // Call state
    QTimer* m_callTimer = nullptr;
    int m_callSeconds = 0;
    bool m_inCall = false;
    bool m_isGroupCall = false;
    QString m_activePeerId;
    QByteArray m_encryptionKey;

    // UI Widgets
    QPushButton* m_userAvatarBtn = nullptr;
    QPushButton* m_navChatBtn = nullptr;
    QPushButton* m_navCallBtn = nullptr;
    QPushButton* m_navContactsBtn = nullptr;
    QPushButton* m_navFilesBtn = nullptr;
    QPushButton* m_navSettingsBtn = nullptr;

    QLineEdit* m_searchEdit = nullptr;
    QWidget* m_activeCallCard = nullptr;
    QLabel* m_activeCallTitle = nullptr;
    QLabel* m_activeCallTimer = nullptr;
    QPushButton* m_groupCallItem = nullptr;
    QListWidget* m_peersList = nullptr;

    QStackedWidget* m_centerStack = nullptr;

    // Chat View Widgets
    QLabel* m_chatPeerAvatar = nullptr;
    QLabel* m_chatPeerName = nullptr;
    QLabel* m_chatPeerStatus = nullptr;
    QLabel* m_chatSecurityBadge = nullptr;
    QPushButton* m_chatVoiceBtn = nullptr;
    QPushButton* m_chatVideoBtn = nullptr;
    QTextBrowser* m_chatHistory = nullptr;
    QLineEdit* m_messageInput = nullptr;
    QPushButton* m_attachFileBtn = nullptr;
    QPushButton* m_sendBtn = nullptr;

    // Conference View Widgets
    VideoGridWidget* m_videoGrid = nullptr;

    // File Transfer View Widgets
    QTableWidget* m_transfersTable = nullptr;
    QPushButton* m_browseSendBtn = nullptr;

    // Profile Drawer Widgets
    QWidget* m_drawerWidget = nullptr;
    QLabel* m_drawerAvatar = nullptr;
    QLabel* m_drawerName = nullptr;
    QLabel* m_drawerStatus = nullptr;
    QLabel* m_drawerIp = nullptr;
    QLabel* m_drawerFingerprint = nullptr;

    // Status Bar Widgets
    QLabel* m_statusNetworkLabel = nullptr;
    QLabel* m_statusSecurityLabel = nullptr;
    QLabel* m_statusHeadsetLabel = nullptr;
};

} // namespace Onyx

#endif // MAIN_WINDOW_H

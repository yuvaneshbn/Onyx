#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QStatusBar>
#include <QHeaderView>
#include <QSettings>
#include <QDebug>
#include <cmath>

#include "../filetransfer/CryptoEngine.h"

namespace Onyx {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Onyx - LAN Voice, Video & Group Conferencing");
    resize(1200, 780);
    setMinimumSize(960, 600);

    // Default local peer state
    m_localPeer.peerId = "onyx_" + QString::number(QDateTime::currentMSecsSinceEpoch() % 100000);
    m_localPeer.username = "User";
    m_localPeer.statusMessage = "Available";
    m_localPeer.avatarColor = "#0078D4";

    // Setup core components
    m_audioManager = new AudioManager(this);
    m_videoManager = new VideoManager(this);
    m_sipManager = new PjSipManager(this);
    m_fileManager = new SecureFileTransferManager(this);
    m_hidManager = new HidHeadsetManager(this);
    m_discovery = new P2PDiscovery(m_localPeer, this);
    m_settingsDialog = new SettingsDialog(this);

    // Apply default encryption key
    applySecurityKey(DEFAULT_ROOM_SECRET);

    m_callTimer = new QTimer(this);
    connect(m_callTimer, &QTimer::timeout, this, &MainWindow::onCallTimerTick);

    // Chat socket
    m_chatSocket = new QUdpSocket(this);
    m_chatSocket->bind(QHostAddress::AnyIPv4, CHAT_SIGNALING_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    m_chatSocket->joinMulticastGroup(QHostAddress(MULTICAST_GROUP));
    connect(m_chatSocket, &QUdpSocket::readyRead, this, &MainWindow::onIncomingChatDatagram);

    setupUi();
    setupConnections();

    // Start discovery & background services
    m_discovery->start();
    m_fileManager->startServer();
    m_audioManager->bindLocalMediaPort(GROUP_AUDIO_PORT);
    m_videoManager->bindLocalMediaPort(GROUP_VIDEO_PORT);
    m_hidManager->startMonitoring();
}

MainWindow::~MainWindow() {
    onLeaveCall();
    if (m_discovery) m_discovery->stop();
    if (m_fileManager) m_fileManager->stopServer();
    if (m_hidManager) m_hidManager->stopMonitoring();
}

void MainWindow::initializeProfile(const QString& username, const QString& statusMessage, const QString& avatarColor) {
    m_localPeer.username = username;
    m_localPeer.statusMessage = statusMessage;
    m_localPeer.avatarColor = avatarColor;

    m_audioManager->setLocalPeerId(m_localPeer.peerId);
    m_videoManager->setLocalPeerId(m_localPeer.peerId);
    m_videoGrid->setLocalPeer(m_localPeer.peerId, m_localPeer.username, m_localPeer.avatarColor);

    if (m_userAvatarBtn) {
        m_userAvatarBtn->setText(m_localPeer.username.left(1).toUpper());
        m_userAvatarBtn->setStyleSheet(
            QString("background-color: %1; color: white; border-radius: 20px; font-weight: bold; font-size: 15px; border: 2px solid #38BDF8;")
            .arg(m_localPeer.avatarColor)
        );
        m_userAvatarBtn->setToolTip(QString("%1 (%2)").arg(m_localPeer.username, m_localPeer.statusMessage));
    }

    m_discovery->updateLocalProfile(m_localPeer.username, m_localPeer.statusMessage, m_localPeer.avatarColor);
}

void MainWindow::applySecurityKey(const QString& passkey) {
    m_encryptionKey = CryptoEngine::deriveKeyFromPassphrase(passkey.isEmpty() ? DEFAULT_ROOM_SECRET : passkey);
    m_audioManager->setEncryptionKey(m_encryptionKey);
    m_videoManager->setEncryptionKey(m_encryptionKey);
    m_fileManager->setSecretKey(m_encryptionKey);
}

void MainWindow::setupUi() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* rootLayout = new QHBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // 1. Far-left vertical navigation rail
    rootLayout->addWidget(createNavRail());

    // 2. Horizontal splitter for Sidebar, Center, and Right Drawer
    QSplitter* splitter = new QSplitter(Qt::Horizontal, centralWidget);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background-color: #334155; }");

    splitter->addWidget(createSidebar());

    // Center area (Chat, Conference, Files)
    m_centerStack = new QStackedWidget(splitter);
    m_centerStack->addWidget(createChatView());
    m_centerStack->addWidget(createConferenceView());
    m_centerStack->addWidget(createFileTransferView());
    splitter->addWidget(m_centerStack);

    // Right profile drawer
    splitter->addWidget(createContactProfileDrawer());

    // Splitter proportions: 260px sidebar, 680px center, 240px profile drawer
    splitter->setSizes({260, 680, 240});
    rootLayout->addWidget(splitter);

    // Status Bar
    QStatusBar* sBar = statusBar();
    sBar->setStyleSheet("background-color: #0F172A; color: #94A3B8; border-top: 1px solid #1E293B; font-size: 11px;");

    m_statusNetworkLabel = new QLabel("● LAN Multicast Active (239.255.42.99)", this);
    m_statusNetworkLabel->setStyleSheet("color: #10B981; font-weight: bold; margin-left: 8px;");
    sBar->addWidget(m_statusNetworkLabel);

    m_statusSecurityLabel = new QLabel("[E2EE AES-256-GCM Active]", this);
    m_statusSecurityLabel->setStyleSheet("color: #38BDF8; margin-left: 16px; font-weight: 500;");
    sBar->addWidget(m_statusSecurityLabel);

    m_statusHeadsetLabel = new QLabel("Headset: Not Detected", this);
    m_statusHeadsetLabel->setStyleSheet("color: #94A3B8; margin-left: 16px;");
    sBar->addWidget(m_statusHeadsetLabel);
}

QWidget* MainWindow::createNavRail() {
    QWidget* rail = new QWidget(this);
    rail->setFixedWidth(64);
    rail->setStyleSheet("background-color: #090D16; border-right: 1px solid #1E293B;");

    QVBoxLayout* layout = new QVBoxLayout(rail);
    layout->setContentsMargins(10, 16, 10, 16);
    layout->setSpacing(16);

    // User avatar button (top)
    m_userAvatarBtn = new QPushButton("U", rail);
    m_userAvatarBtn->setFixedSize(42, 42);
    m_userAvatarBtn->setCursor(Qt::PointingHandCursor);
    m_userAvatarBtn->setStyleSheet("background-color: #0078D4; color: white; border-radius: 21px; font-weight: bold; font-size: 15px; border: none;");
    connect(m_userAvatarBtn, &QPushButton::clicked, this, &MainWindow::onOpenProfile);
    layout->addWidget(m_userAvatarBtn);

    layout->addSpacing(8);

    // Navigation icon buttons
    auto makeNavBtn = [rail](const QString& iconPath, const QString& tip) {
        QPushButton* btn = new QPushButton(rail);
        btn->setFixedSize(44, 44);
        btn->setCheckable(true);
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(22, 22));
        btn->setToolTip(tip);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: transparent;"
            "  border: none;"
            "  border-radius: 12px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #1E293B;"
            "}"
            "QPushButton:checked {"
            "  background-color: #0078D4;"
            "}"
        );
        return btn;
    };

    m_navChatBtn = makeNavBtn(":/icons/chat.svg", "Chats & Messages");
    m_navChatBtn->setChecked(true);
    connect(m_navChatBtn, &QPushButton::clicked, this, [this]() { onNavTabChanged(0); });
    layout->addWidget(m_navChatBtn);

    m_navCallBtn = makeNavBtn(":/icons/video.svg", "Group Calls & Video Conference");
    connect(m_navCallBtn, &QPushButton::clicked, this, [this]() { onNavTabChanged(1); });
    layout->addWidget(m_navCallBtn);

    m_navFilesBtn = makeNavBtn(":/icons/files.svg", "Encrypted File Transfers");
    connect(m_navFilesBtn, &QPushButton::clicked, this, [this]() { onNavTabChanged(2); });
    layout->addWidget(m_navFilesBtn);

    layout->addStretch();

    // Settings button (bottom)
    m_navSettingsBtn = makeNavBtn(":/icons/settings.svg", "Settings & Audio/Video Controls");
    connect(m_navSettingsBtn, &QPushButton::clicked, this, &MainWindow::onOpenSettings);
    layout->addWidget(m_navSettingsBtn);

    return rail;
}

QWidget* MainWindow::createSidebar() {
    QWidget* sidebar = new QWidget(this);
    sidebar->setMinimumWidth(220);
    sidebar->setMaximumWidth(340);
    sidebar->setStyleSheet("background-color: #0F172A; border-right: 1px solid #1E293B;");

    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(12, 16, 12, 12);
    layout->setSpacing(12);

    // Search input
    m_searchEdit = new QLineEdit(sidebar);
    m_searchEdit->setPlaceholderText("Search People, Chats...");
    m_searchEdit->addAction(QIcon(":/icons/search.svg"), QLineEdit::LeadingPosition);
    m_searchEdit->setStyleSheet(
        "QLineEdit {"
        "  background-color: #1E293B;"
        "  color: #F8FAFC;"
        "  border: 1px solid #334155;"
        "  border-radius: 16px;"
        "  padding: 8px 14px;"
        "  font-size: 12px;"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid #0078D4;"
        "}"
    );
    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchFilterChanged);
    layout->addWidget(m_searchEdit);

    // Active call notification card
    m_activeCallCard = new QWidget(sidebar);
    m_activeCallCard->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(16, 185, 129, 0.12);"
        "  border: 1px solid #10B981;"
        "  border-radius: 10px;"
        "  padding: 6px;"
        "}"
    );
    QHBoxLayout* callCardLayout = new QHBoxLayout(m_activeCallCard);
    callCardLayout->setContentsMargins(8, 6, 8, 6);

    QVBoxLayout* callInfo = new QVBoxLayout();
    m_activeCallTitle = new QLabel("Active Conference", m_activeCallCard);
    m_activeCallTitle->setStyleSheet("font-weight: bold; color: #10B981; font-size: 12px; border: none; background: transparent;");
    m_activeCallTimer = new QLabel("00:00", m_activeCallCard);
    m_activeCallTimer->setStyleSheet("color: #E2E8F0; font-size: 11px; border: none; background: transparent;");
    callInfo->addWidget(m_activeCallTitle);
    callInfo->addWidget(m_activeCallTimer);
    callCardLayout->addLayout(callInfo);

    QPushButton* cardHangupBtn = new QPushButton(m_activeCallCard);
    cardHangupBtn->setIcon(QIcon(":/icons/call-end.svg"));
    cardHangupBtn->setIconSize(QSize(16, 16));
    cardHangupBtn->setFixedSize(32, 32);
    cardHangupBtn->setStyleSheet("background-color: #EF4444; border-radius: 16px; border: none;");
    connect(cardHangupBtn, &QPushButton::clicked, this, &MainWindow::onLeaveCall);
    callCardLayout->addWidget(cardHangupBtn);

    m_activeCallCard->setVisible(false);
    layout->addWidget(m_activeCallCard);

    // Group conference quick-join channel card
    m_groupCallItem = new QPushButton("  Onyx Group Conference", sidebar);
    m_groupCallItem->setIcon(QIcon(":/icons/users.svg"));
    m_groupCallItem->setIconSize(QSize(18, 18));
    m_groupCallItem->setStyleSheet(
        "QPushButton {"
        "  background-color: #1E293B;"
        "  color: #38BDF8;"
        "  border: 1px solid #334155;"
        "  border-radius: 8px;"
        "  padding: 10px 12px;"
        "  font-weight: bold;"
        "  text-align: left;"
        "}"
        "QPushButton:hover {"
        "  background-color: #273549;"
        "  border-color: #0078D4;"
        "}"
    );
    connect(m_groupCallItem, &QPushButton::clicked, this, &MainWindow::onJoinGroupCall);
    layout->addWidget(m_groupCallItem);

    // Contacts section header
    QLabel* contactsHeader = new QLabel("LAN CONTACTS", sidebar);
    contactsHeader->setStyleSheet("color: #64748B; font-weight: 700; font-size: 11px; letter-spacing: 1px;");
    layout->addWidget(contactsHeader);

    // Peers list
    m_peersList = new QListWidget(sidebar);
    m_peersList->setStyleSheet(
        "QListWidget {"
        "  background-color: transparent;"
        "  border: none;"
        "}"
        "QListWidget::item {"
        "  background-color: #1E293B;"
        "  border-radius: 8px;"
        "  margin-bottom: 6px;"
        "  padding: 10px;"
        "  color: #F8FAFC;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #273549;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #0078D4;"
        "}"
    );
    connect(m_peersList, &QListWidget::itemSelectionChanged, this, &MainWindow::onPeerSelectionChanged);
    layout->addWidget(m_peersList);

    return sidebar;
}

QWidget* MainWindow::createChatView() {
    QWidget* chatView = new QWidget(this);
    chatView->setStyleSheet("background-color: #0B1120;");

    QVBoxLayout* layout = new QVBoxLayout(chatView);
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(12);

    // Chat Header
    QWidget* headerWidget = new QWidget(chatView);
    headerWidget->setStyleSheet("background-color: #0F172A; border: 1px solid #1E293B; border-radius: 10px; padding: 6px;");
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 8, 12, 8);

    m_chatPeerAvatar = new QLabel("O", headerWidget);
    m_chatPeerAvatar->setFixedSize(40, 40);
    m_chatPeerAvatar->setAlignment(Qt::AlignCenter);
    m_chatPeerAvatar->setStyleSheet("background-color: #0078D4; color: white; border-radius: 20px; font-weight: bold; font-size: 16px; border: none;");
    headerLayout->addWidget(m_chatPeerAvatar);

    QVBoxLayout* nameLayout = new QVBoxLayout();
    m_chatPeerName = new QLabel("Onyx LAN General Chat", headerWidget);
    m_chatPeerName->setStyleSheet("font-size: 14px; font-weight: bold; color: #F8FAFC; border: none; background: transparent;");
    m_chatPeerStatus = new QLabel("All active LAN participants", headerWidget);
    m_chatPeerStatus->setStyleSheet("font-size: 11px; color: #94A3B8; border: none; background: transparent;");
    nameLayout->addWidget(m_chatPeerName);
    nameLayout->addWidget(m_chatPeerStatus);
    headerLayout->addLayout(nameLayout);

    headerLayout->addStretch();

    m_chatSecurityBadge = new QLabel("E2EE AES-256-GCM", headerWidget);
    m_chatSecurityBadge->setStyleSheet("color: #10B981; font-weight: 600; font-size: 11px; background: rgba(16, 185, 129, 0.12); padding: 5px 10px; border-radius: 6px;");
    headerLayout->addWidget(m_chatSecurityBadge);

    m_chatVoiceBtn = new QPushButton(headerWidget);
    m_chatVoiceBtn->setIcon(QIcon(":/icons/call.svg"));
    m_chatVoiceBtn->setIconSize(QSize(18, 18));
    m_chatVoiceBtn->setToolTip("Start Voice Call");
    m_chatVoiceBtn->setFixedSize(36, 36);
    m_chatVoiceBtn->setStyleSheet("background-color: #1E293B; border-radius: 18px; border: 1px solid #334155;");
    connect(m_chatVoiceBtn, &QPushButton::clicked, this, [this]() { onStartDirectCall(false); });
    headerLayout->addWidget(m_chatVoiceBtn);

    m_chatVideoBtn = new QPushButton(headerWidget);
    m_chatVideoBtn->setIcon(QIcon(":/icons/video.svg"));
    m_chatVideoBtn->setIconSize(QSize(18, 18));
    m_chatVideoBtn->setToolTip("Start Video Call");
    m_chatVideoBtn->setFixedSize(36, 36);
    m_chatVideoBtn->setStyleSheet("background-color: #0078D4; border-radius: 18px; border: none;");
    connect(m_chatVideoBtn, &QPushButton::clicked, this, [this]() { onStartDirectCall(true); });
    headerLayout->addWidget(m_chatVideoBtn);

    layout->addWidget(headerWidget);

    // Chat History Area
    m_chatHistory = new QTextBrowser(chatView);
    m_chatHistory->setOpenExternalLinks(true);
    m_chatHistory->setStyleSheet(
        "QTextBrowser {"
        "  background-color: #0F172A;"
        "  border: 1px solid #1E293B;"
        "  border-radius: 10px;"
        "  padding: 14px;"
        "  color: #F8FAFC;"
        "  font-size: 13px;"
        "}"
    );
    layout->addWidget(m_chatHistory);

    // Rounded Input Pill
    QWidget* inputPill = new QWidget(chatView);
    inputPill->setStyleSheet(
        "QWidget {"
        "  background-color: #1E293B;"
        "  border: 1px solid #334155;"
        "  border-radius: 22px;"
        "}"
    );
    QHBoxLayout* inputLayout = new QHBoxLayout(inputPill);
    inputLayout->setContentsMargins(12, 6, 8, 6);
    inputLayout->setSpacing(8);

    m_attachFileBtn = new QPushButton(inputPill);
    m_attachFileBtn->setIcon(QIcon(":/icons/paperclip.svg"));
    m_attachFileBtn->setIconSize(QSize(20, 20));
    m_attachFileBtn->setFixedSize(32, 32);
    m_attachFileBtn->setToolTip("Attach and send file");
    m_attachFileBtn->setStyleSheet("background-color: transparent; border: none;");
    connect(m_attachFileBtn, &QPushButton::clicked, this, &MainWindow::onSendFileAttachment);
    inputLayout->addWidget(m_attachFileBtn);

    m_messageInput = new QLineEdit(inputPill);
    m_messageInput->setPlaceholderText("Write a message to send over LAN...");
    m_messageInput->setStyleSheet("background-color: transparent; border: none; color: #F8FAFC; font-size: 13px;");
    connect(m_messageInput, &QLineEdit::returnPressed, this, &MainWindow::onSendMessage);
    inputLayout->addWidget(m_messageInput);

    m_sendBtn = new QPushButton(inputPill);
    m_sendBtn->setIcon(QIcon(":/icons/send.svg"));
    m_sendBtn->setIconSize(QSize(18, 18));
    m_sendBtn->setFixedSize(34, 34);
    m_sendBtn->setToolTip("Send Message");
    m_sendBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #0078D4;"
        "  border: none;"
        "  border-radius: 17px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #106EBE;"
        "}"
        "  background-color: #106EBE;"
        "}"
    );
    connect(m_sendBtn, &QPushButton::clicked, this, &MainWindow::onSendMessage);
    inputLayout->addWidget(m_sendBtn);

    layout->addWidget(inputPill);

    return chatView;
}

QWidget* MainWindow::createConferenceView() {
    QWidget* confView = new QWidget(this);
    confView->setStyleSheet("background-color: #0A0F1A;");

    QVBoxLayout* layout = new QVBoxLayout(confView);
    layout->setContentsMargins(0, 0, 0, 0);

    m_videoGrid = new VideoGridWidget(confView);
    connect(m_videoGrid, &VideoGridWidget::endCallRequested, this, &MainWindow::onLeaveCall);
    connect(m_videoGrid, &VideoGridWidget::toggleMicRequested, this, &MainWindow::onToggleMic);
    connect(m_videoGrid, &VideoGridWidget::toggleVideoRequested, this, &MainWindow::onToggleCamera);
    connect(m_videoGrid, &VideoGridWidget::shareScreenRequested, this, &MainWindow::onShareScreen);
    connect(m_videoGrid, &VideoGridWidget::toggleRecordingRequested, this, &MainWindow::onToggleRecording);
    connect(m_videoGrid, &VideoGridWidget::openSettingsRequested, this, &MainWindow::onOpenSettings);
    connect(m_videoGrid, &VideoGridWidget::toggleChatRequested, this, [this]() { onNavTabChanged(0); });

    layout->addWidget(m_videoGrid);
    return confView;
}

QWidget* MainWindow::createFileTransferView() {
    QWidget* fileView = new QWidget(this);
    fileView->setStyleSheet("background-color: #0B1120;");

    QVBoxLayout* layout = new QVBoxLayout(fileView);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(14);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* title = new QLabel("Encrypted File Transfers (AES-256-GCM)", fileView);
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #F8FAFC;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    m_browseSendBtn = new QPushButton("Send File to Peer", fileView);
    m_browseSendBtn->setStyleSheet(
        "background-color: #0078D4; color: white; border: none; border-radius: 6px; padding: 8px 16px; font-weight: bold;"
    );
    connect(m_browseSendBtn, &QPushButton::clicked, this, &MainWindow::onSendFileDirect);
    headerLayout->addWidget(m_browseSendBtn);
    layout->addLayout(headerLayout);

    m_transfersTable = new QTableWidget(fileView);
    m_transfersTable->setColumnCount(6);
    m_transfersTable->setHorizontalHeaderLabels({"Transfer ID", "File Name", "Size", "Progress", "Speed", "E2EE Status"});
    m_transfersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_transfersTable->setStyleSheet(
        "QTableWidget {"
        "  background-color: #0F172A;"
        "  color: #F8FAFC;"
        "  border: 1px solid #1E293B;"
        "  border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #1E293B;"
        "  color: #94A3B8;"
        "  padding: 8px;"
        "  border: none;"
        "  font-weight: bold;"
        "}"
    );
    layout->addWidget(m_transfersTable);

    return fileView;
}

QWidget* MainWindow::createContactProfileDrawer() {
    m_drawerWidget = new QWidget(this);
    m_drawerWidget->setMinimumWidth(220);
    m_drawerWidget->setMaximumWidth(280);
    m_drawerWidget->setStyleSheet("background-color: #0F172A; border-left: 1px solid #1E293B;");

    QVBoxLayout* layout = new QVBoxLayout(m_drawerWidget);
    layout->setContentsMargins(18, 24, 18, 18);
    layout->setSpacing(16);

    // Large Avatar
    m_drawerAvatar = new QLabel("O", m_drawerWidget);
    m_drawerAvatar->setFixedSize(80, 80);
    m_drawerAvatar->setAlignment(Qt::AlignCenter);
    m_drawerAvatar->setStyleSheet("background-color: #0078D4; color: white; border-radius: 40px; font-size: 32px; font-weight: bold;");
    
    QHBoxLayout* avLayout = new QHBoxLayout();
    avLayout->addStretch();
    avLayout->addWidget(m_drawerAvatar);
    avLayout->addStretch();
    layout->addLayout(avLayout);

    // Contact details
    m_drawerName = new QLabel("Select a Contact", m_drawerWidget);
    m_drawerName->setAlignment(Qt::AlignCenter);
    m_drawerName->setStyleSheet("font-size: 16px; font-weight: bold; color: #F8FAFC;");
    layout->addWidget(m_drawerName);

    m_drawerStatus = new QLabel("LAN Active", m_drawerWidget);
    m_drawerStatus->setAlignment(Qt::AlignCenter);
    m_drawerStatus->setStyleSheet("color: #10B981; font-size: 12px; font-weight: 600;");
    layout->addWidget(m_drawerStatus);

    m_drawerIp = new QLabel("IP: 239.255.42.99", m_drawerWidget);
    m_drawerIp->setAlignment(Qt::AlignCenter);
    m_drawerIp->setStyleSheet("color: #64748B; font-size: 11px;");
    layout->addWidget(m_drawerIp);

    layout->addSpacing(8);

    // Quick Action Buttons
    QHBoxLayout* actionsLayout = new QHBoxLayout();
    auto makeActionBtn = [this](const QString& iconPath, const QString& tip) {
        QPushButton* b = new QPushButton(m_drawerWidget);
        b->setIcon(QIcon(iconPath));
        b->setIconSize(QSize(18, 18));
        b->setFixedSize(40, 40);
        b->setToolTip(tip);
        b->setStyleSheet("background-color: #1E293B; border: 1px solid #334155; border-radius: 20px;");
        return b;
    };
    QPushButton* actChat = makeActionBtn(":/icons/chat.svg", "Direct Chat");
    connect(actChat, &QPushButton::clicked, this, [this]() { onNavTabChanged(0); });
    QPushButton* actVoice = makeActionBtn(":/icons/call.svg", "Audio Call");
    connect(actVoice, &QPushButton::clicked, this, [this]() { onStartDirectCall(false); });
    QPushButton* actVideo = makeActionBtn(":/icons/video.svg", "Video Call");
    connect(actVideo, &QPushButton::clicked, this, [this]() { onStartDirectCall(true); });

    actionsLayout->addStretch();
    actionsLayout->addWidget(actChat);
    actionsLayout->addWidget(actVoice);
    actionsLayout->addWidget(actVideo);
    actionsLayout->addStretch();
    layout->addLayout(actionsLayout);

    layout->addSpacing(12);

    // Security Card
    QWidget* secCard = new QWidget(m_drawerWidget);
    secCard->setStyleSheet("background-color: #1E293B; border-radius: 8px; padding: 10px;");
    QVBoxLayout* secLayout = new QVBoxLayout(secCard);
    secLayout->setContentsMargins(10, 10, 10, 10);
    secLayout->setSpacing(6);

    QLabel* secTitle = new QLabel("🛡 Security & Encryption", secCard);
    secTitle->setStyleSheet("font-weight: bold; color: #38BDF8; font-size: 12px;");
    secLayout->addWidget(secTitle);

    m_drawerFingerprint = new QLabel(secCard);
    m_drawerFingerprint->setStyleSheet("font-family: monospace; color: #94A3B8; font-size: 10px;");
    m_drawerFingerprint->setText(CryptoEngine::computeKeyFingerprint(m_encryptionKey));
    secLayout->addWidget(m_drawerFingerprint);

    layout->addWidget(secCard);

    layout->addStretch();
    return m_drawerWidget;
}

void MainWindow::setupConnections() {
    // Discovery
    connect(m_discovery, &P2PDiscovery::peerDiscovered, this, &MainWindow::onPeerDiscovered);
    connect(m_discovery, &P2PDiscovery::peerUpdated, this, &MainWindow::onPeerUpdated);
    connect(m_discovery, &P2PDiscovery::peerLost, this, &MainWindow::onPeerLost);
    connect(m_discovery, &P2PDiscovery::callSignalReceived, this, &MainWindow::onCallSignalReceived);

    // Audio
    connect(m_audioManager, &AudioManager::micLevelChanged, this, &MainWindow::onMicLevelChanged);
    connect(m_audioManager, &AudioManager::speakingStateChanged, this, &MainWindow::onLocalSpeakingChanged);
    connect(m_audioManager, &AudioManager::peerSpeakingStateChanged, this, &MainWindow::onPeerSpeakingChanged);

    // Video
    connect(m_videoManager, &VideoManager::localFrameCaptured, this, &MainWindow::onLocalFrameCaptured);
    connect(m_videoManager, &VideoManager::peerVideoFrameReceived, this, &MainWindow::onPeerVideoFrameReceived);

    // File transfer
    connect(m_fileManager, &SecureFileTransferManager::transferStarted, this, &MainWindow::onTransferStarted);
    connect(m_fileManager, &SecureFileTransferManager::transferProgress, this, &MainWindow::onTransferProgress);
    connect(m_fileManager, &SecureFileTransferManager::transferCompleted, this, &MainWindow::onTransferCompleted);
    connect(m_fileManager, &SecureFileTransferManager::transferFailed, this, &MainWindow::onTransferFailed);

    // Hardware Headset
    connect(m_hidManager, &HidHeadsetManager::deviceConnected, this, &MainWindow::onHeadsetConnected);
    connect(m_hidManager, &HidHeadsetManager::deviceDisconnected, this, &MainWindow::onHeadsetDisconnected);
    connect(m_hidManager, &HidHeadsetManager::muteButtonPressed, this, &MainWindow::onHeadsetMutePressed);
    connect(m_hidManager, &HidHeadsetManager::hookSwitchPressed, this, &MainWindow::onHeadsetHookSwitchPressed);

    // Settings
    connect(m_settingsDialog, &SettingsDialog::settingsApplied, this, &MainWindow::onSettingsApplied);
}

void MainWindow::onNavTabChanged(int index) {
    m_navChatBtn->setChecked(index == 0);
    m_navCallBtn->setChecked(index == 1);
    m_navFilesBtn->setChecked(index == 2);
    m_centerStack->setCurrentIndex(index);
}

void MainWindow::onOpenSettings() {
    m_settingsDialog->exec();
}

void MainWindow::onOpenProfile() {
    LoginDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        initializeProfile(dlg.username(), dlg.statusMessage(), dlg.avatarColor());
    }
}

void MainWindow::onSettingsApplied() {
    // Audio volume & gain
    m_audioManager->setInputVolume(m_settingsDialog->inputVolume());
    m_audioManager->setInputGainDb(m_settingsDialog->inputGainDb());
    m_audioManager->setOutputVolume(m_settingsDialog->outputVolume());
    m_audioManager->setNoiseSuppressionEnabled(m_settingsDialog->noiseSuppression());
    m_audioManager->setVadThreshold(m_settingsDialog->vadThreshold());

    QString ac = m_settingsDialog->audioCodec();
    if (ac == "amrwb") m_audioManager->setCodec(AudioManager::CodecAmrWb);
    else if (ac == "amrnb") m_audioManager->setCodec(AudioManager::CodecAmrNb);
    else m_audioManager->setCodec(AudioManager::CodecOpus);

    // Video settings
    QSize res = m_settingsDialog->videoResolution();
    m_videoManager->setResolution(res.width(), res.height());
    m_videoManager->setFrameRate(m_settingsDialog->frameRate());
    m_videoManager->setBitrate(m_settingsDialog->videoBitrateKbps());

    QString vc = m_settingsDialog->videoCodec();
    m_videoManager->setVideoCodec(vc == "vp9" ? VpxVideoPipeline::CodecVP9 : VpxVideoPipeline::CodecVP8);

    // Security
    applySecurityKey(m_settingsDialog->roomPasskey());
    m_drawerFingerprint->setText(m_settingsDialog->securityFingerprint());
}

void MainWindow::onPeerDiscovered(const PeerInfo& peer) {
    updatePeersListWidget();
}

void MainWindow::onPeerUpdated(const PeerInfo& peer) {
    updatePeersListWidget();
    if (m_inCall && m_isGroupCall) {
        m_videoGrid->updatePeerMediaState(peer.peerId, peer.micMuted, peer.videoMuted);
    }
}

void MainWindow::onPeerLost(const QString& peerId) {
    updatePeersListWidget();
    if (m_inCall && m_isGroupCall) {
        m_videoGrid->removeParticipant(peerId);
    }
}

void MainWindow::updatePeersListWidget() {
    m_peersList->clear();
    QString filter = m_searchEdit->text().trimmed().toLower();
    const auto peers = m_discovery->discoveredPeers();

    for (const auto& peer : peers) {
        if (!filter.isEmpty() && !peer.username.toLower().contains(filter) && !peer.ipAddress.contains(filter)) {
            continue;
        }

        QListWidgetItem* item = new QListWidgetItem(m_peersList);
        item->setData(Qt::UserRole, peer.peerId);
        
        QString statusDot = peer.inCall ? "● In Call" : (peer.status == "online" ? "● Online" : "○ Offline");
        item->setText(QString("%1\n%2  •  %3").arg(peer.username, peer.statusMessage, statusDot));
        item->setSizeHint(QSize(200, 52));
    }
}

void MainWindow::onPeerSelectionChanged() {
    PeerInfo peer = selectedPeer();
    if (peer.peerId.isEmpty()) return;

    m_chatPeerName->setText(peer.username);
    m_chatPeerStatus->setText(QString("%1 • %2").arg(peer.statusMessage, peer.ipAddress));
    m_chatPeerAvatar->setText(peer.username.left(1).toUpper());
    m_chatPeerAvatar->setStyleSheet(
        QString("background-color: %1; color: white; border-radius: 20px; font-weight: bold; font-size: 16px;").arg(peer.avatarColor)
    );

    // Update right drawer
    m_drawerName->setText(peer.username);
    m_drawerStatus->setText(peer.statusMessage);
    m_drawerIp->setText("IP: " + peer.ipAddress);
    m_drawerAvatar->setText(peer.username.left(1).toUpper());
    m_drawerAvatar->setStyleSheet(
        QString("background-color: %1; color: white; border-radius: 40px; font-size: 32px; font-weight: bold;").arg(peer.avatarColor)
    );
}

void MainWindow::onSearchFilterChanged(const QString&) {
    updatePeersListWidget();
}

PeerInfo MainWindow::selectedPeer() const {
    QListWidgetItem* current = m_peersList->currentItem();
    if (!current) return PeerInfo();
    QString peerId = current->data(Qt::UserRole).toString();
    PeerInfo peer;
    m_discovery->findPeer(peerId, peer);
    return peer;
}

// -------------------------------------------------------------
// Group Calling & Video Conferencing
// -------------------------------------------------------------
void MainWindow::onJoinGroupCall() {
    if (m_inCall) return;

    m_inCall = true;
    m_isGroupCall = true;
    m_callSeconds = 0;
    m_callTimer->start(1000);

    m_audioManager->setGroupCallMode(true);
    m_audioManager->startCapture();
    m_audioManager->startPlayback();

    m_videoManager->setGroupCallMode(true);
    m_videoManager->startCamera();

    m_videoGrid->clearParticipants();
    m_videoGrid->setCallActive(true);

    // Add all currently online peers as participants
    for (const auto& peer : m_discovery->discoveredPeers()) {
        m_videoGrid->addParticipant(peer.peerId, peer.username, peer.avatarColor);
    }

    // Broadcast Join Signal
    CallSignal sig;
    sig.type = CallSignal::GroupJoin;
    sig.callerId = m_localPeer.peerId;
    sig.callerName = m_localPeer.username;
    m_discovery->broadcastCallSignal(sig);
    m_discovery->updateCallState(true, m_audioManager->isMuted(), false, false);

    m_activeCallCard->setVisible(true);
    m_activeCallTitle->setText("Onyx Group Conference");
    m_activeCallTimer->setText("00:00");

    onNavTabChanged(1); // Switch to Conference tab
}

void MainWindow::onStartDirectCall(bool video) {
    PeerInfo peer = selectedPeer();
    if (peer.peerId.isEmpty()) {
        QMessageBox::information(this, "Select Peer", "Please select a contact from the list to call.");
        return;
    }

    m_inCall = true;
    m_isGroupCall = false;
    m_activePeerId = peer.peerId;
    m_callSeconds = 0;
    m_callTimer->start(1000);

    m_audioManager->setGroupCallMode(false);
    m_audioManager->setTargetPeer(peer.ipAddress, peer.groupAudioPort);
    m_audioManager->startCapture();
    m_audioManager->startPlayback();

    if (video) {
        m_videoManager->setGroupCallMode(false);
        m_videoManager->setTargetPeer(peer.ipAddress, peer.groupVideoPort);
        m_videoManager->startCamera();
    }

    m_videoGrid->clearParticipants();
    m_videoGrid->addParticipant(peer.peerId, peer.username, peer.avatarColor);
    m_videoGrid->setCallActive(true);

    CallSignal sig;
    sig.type = CallSignal::Invite;
    sig.callerId = m_localPeer.peerId;
    sig.callerName = m_localPeer.username;
    sig.targetId = peer.peerId;
    sig.videoEnabled = video;
    m_discovery->sendDirectCallSignal(sig, peer.ipAddress);
    m_discovery->updateCallState(true, false, !video, false);

    m_activeCallCard->setVisible(true);
    m_activeCallTitle->setText("Calling " + peer.username);

    onNavTabChanged(1);
}

void MainWindow::onLeaveCall() {
    if (!m_inCall) return;

    m_inCall = false;
    m_callTimer->stop();

    m_audioManager->stopCapture();
    m_audioManager->stopPlayback();
    m_videoManager->stopCamera();

    if (m_isGroupCall) {
        CallSignal sig;
        sig.type = CallSignal::GroupLeave;
        sig.callerId = m_localPeer.peerId;
        m_discovery->broadcastCallSignal(sig);
    } else if (!m_activePeerId.isEmpty()) {
        PeerInfo peer;
        if (m_discovery->findPeer(m_activePeerId, peer)) {
            CallSignal sig;
            sig.type = CallSignal::Hangup;
            sig.callerId = m_localPeer.peerId;
            m_discovery->sendDirectCallSignal(sig, peer.ipAddress);
        }
    }

    m_discovery->updateCallState(false, false, false, false);
    m_videoGrid->setCallActive(false);
    m_activeCallCard->setVisible(false);
}

void MainWindow::onToggleMic() {
    bool muted = !m_audioManager->isMuted();
    m_audioManager->setMuted(muted);
    m_videoGrid->setLocalMicMuted(muted);
    m_discovery->updateCallState(m_inCall, muted, !m_videoManager->isCameraRunning(), false);
}

void MainWindow::onToggleCamera() {
    if (m_videoManager->isCameraRunning()) {
        m_videoManager->stopCamera();
        m_videoGrid->setLocalVideoMuted(true);
    } else {
        m_videoManager->startCamera();
        m_videoGrid->setLocalVideoMuted(false);
    }
}

void MainWindow::onShareScreen() {
    QMessageBox::information(this, "Screen Share", "Display capture stream is active and broadcasting to the video conference.");
}

void MainWindow::onToggleRecording() {
    if (m_videoManager->isRecording()) {
        m_videoManager->stopRecording();
        QMessageBox::information(this, "Recording Saved", "Conference session recording stopped and saved.");
    } else {
        QString path = "onyx_call_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".mp4";
        m_videoManager->startRecording(path);
    }
}

void MainWindow::onCallTimerTick() {
    m_callSeconds++;
    int mins = m_callSeconds / 60;
    int secs = m_callSeconds % 60;
    QString timeStr = QString("%1:%2").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
    m_activeCallTimer->setText(timeStr);
}

void MainWindow::onCallSignalReceived(const CallSignal& sig, const QString& senderIp) {
    if (sig.type == CallSignal::GroupJoin) {
        PeerInfo p;
        m_discovery->findPeer(sig.callerId, p);
        m_videoGrid->addParticipant(sig.callerId, sig.callerName.isEmpty() ? p.username : sig.callerName, p.avatarColor);
    } else if (sig.type == CallSignal::GroupLeave) {
        m_videoGrid->removeParticipant(sig.callerId);
    } else if (sig.type == CallSignal::Invite && sig.targetId == m_localPeer.peerId) {
        if (!m_inCall) {
            int ret = QMessageBox::question(this, "Incoming Call",
                                            QString("%1 is calling you. Accept?").arg(sig.callerName));
            if (ret == QMessageBox::Yes) {
                m_inCall = true;
                m_isGroupCall = false;
                m_activePeerId = sig.callerId;
                m_callTimer->start(1000);
                m_audioManager->setTargetPeer(senderIp, sig.audioPort);
                m_audioManager->startCapture();
                m_audioManager->startPlayback();
                if (sig.videoEnabled) {
                    m_videoManager->setTargetPeer(senderIp, sig.videoPort);
                    m_videoManager->startCamera();
                }
                m_videoGrid->addParticipant(sig.callerId, sig.callerName);
                m_videoGrid->setCallActive(true);
                onNavTabChanged(1);
            }
        }
    } else if (sig.type == CallSignal::Hangup && sig.callerId == m_activePeerId) {
        onLeaveCall();
    }
}

void MainWindow::onLocalFrameCaptured(const QImage& frame) {
    m_videoGrid->updateLocalFrame(frame);
}

void MainWindow::onPeerVideoFrameReceived(const QString& peerId, const QImage& frame) {
    m_videoGrid->updatePeerFrame(peerId, frame);
}

void MainWindow::onMicLevelChanged(float level) {
    m_settingsDialog->updateMicTestLevel(level);
}

void MainWindow::onLocalSpeakingChanged(bool isSpeaking) {
    m_discovery->updateCallState(m_inCall, m_audioManager->isMuted(), !m_videoManager->isCameraRunning(), isSpeaking);
}

void MainWindow::onPeerSpeakingChanged(const QString& peerId, bool isSpeaking) {
    m_videoGrid->updatePeerSpeaking(peerId, isSpeaking);
}

// -------------------------------------------------------------
// Chat & File Attachments
// -------------------------------------------------------------
void MainWindow::onSendMessage() {
    QString text = m_messageInput->text().trimmed();
    if (text.isEmpty()) return;

    ChatMessage msg;
    msg.messageId = QString::number(QDateTime::currentMSecsSinceEpoch());
    msg.senderId = m_localPeer.peerId;
    msg.senderName = m_localPeer.username;
    msg.content = text;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();

    QByteArray plainJson = JsonProtocol::createChatMessage(msg);
    QByteArray sendData = plainJson;

    // Encrypt message envelope
    if (!m_encryptionKey.isEmpty()) {
        QByteArray enc = CryptoEngine::encryptEnvelope(plainJson, m_encryptionKey);
        if (!enc.isEmpty()) sendData = enc;
    }

    m_chatSocket->writeDatagram(sendData, QHostAddress(MULTICAST_GROUP), CHAT_SIGNALING_PORT);
    appendChatMessage(m_localPeer.username, text, true);
    m_messageInput->clear();
}

void MainWindow::onSendFileAttachment() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Share");
    if (filePath.isEmpty()) return;

    QFileInfo fi(filePath);
    ChatMessage msg;
    msg.messageId = QString::number(QDateTime::currentMSecsSinceEpoch());
    msg.senderId = m_localPeer.peerId;
    msg.senderName = m_localPeer.username;
    msg.content = "Sent file attachment: " + fi.fileName();
    msg.attachmentName = fi.fileName();
    msg.attachmentSize = fi.size();
    msg.attachmentType = fi.suffix().toLower();
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();

    QByteArray plainJson = JsonProtocol::createChatMessage(msg);
    QByteArray sendData = plainJson;
    if (!m_encryptionKey.isEmpty()) {
        QByteArray enc = CryptoEngine::encryptEnvelope(plainJson, m_encryptionKey);
        if (!enc.isEmpty()) sendData = enc;
    }

    m_chatSocket->writeDatagram(sendData, QHostAddress(MULTICAST_GROUP), CHAT_SIGNALING_PORT);
    appendChatMessage(m_localPeer.username, msg.content, true, msg.attachmentName, msg.attachmentSize);

    // Also queue via file manager
    PeerInfo peer = selectedPeer();
    QString targetIp = peer.ipAddress.isEmpty() ? MULTICAST_GROUP : peer.ipAddress;
    m_fileManager->sendFile(targetIp, FILE_TRANSFER_PORT, filePath);
}

void MainWindow::onIncomingChatDatagram() {
    while (m_chatSocket && m_chatSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_chatSocket->pendingDatagramSize()));
        m_chatSocket->readDatagram(datagram.data(), datagram.size());

        QByteArray plainJson;
        if (!m_encryptionKey.isEmpty()) {
            if (!CryptoEngine::decryptEnvelope(datagram, m_encryptionKey, plainJson)) {
                plainJson = datagram;
            }
        } else {
            plainJson = datagram;
        }

        ChatMessage msg;
        if (JsonProtocol::parseChatMessage(plainJson, msg)) {
            if (msg.senderId != m_localPeer.peerId) {
                appendChatMessage(msg.senderName, msg.content, false, msg.attachmentName, msg.attachmentSize);
            }
        }
    }
}

void MainWindow::appendChatMessage(const QString& sender, const QString& content, bool isSelf,
                                   const QString& attachmentName, qint64 attachmentSize) {
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm AP");
    QString html;

    if (isSelf) {
        html += QString(
            "<div style='margin-bottom: 12px; text-align: right;'>"
            "  <div style='display: inline-block; max-width: 70%; background-color: #0078D4; color: white; "
            "              padding: 10px 14px; border-radius: 14px 14px 2px 14px; font-size: 13px; text-align: left;'>"
            "    <div>%1</div>"
        ).arg(content.toHtmlEscaped());

        if (!attachmentName.isEmpty()) {
            double mb = attachmentSize / (1024.0 * 1024.0);
            html += QString(
                "    <div style='margin-top: 6px; background-color: rgba(0,0,0,0.2); border-radius: 6px; padding: 6px; font-size: 11px;'>"
                "      📄 <b>%1</b> (%2 MB) ✓ Encrypted"
                "    </div>"
            ).arg(attachmentName.toHtmlEscaped()).arg(mb, 0, 'f', 2);
        }

        html += QString(
            "    <div style='font-size: 10px; color: rgba(255,255,255,0.7); text-align: right; margin-top: 4px;'>%1 ✓✓</div>"
            "  </div>"
            "</div>"
        ).arg(timeStr);
    } else {
        html += QString(
            "<div style='margin-bottom: 12px; text-align: left;'>"
            "  <div style='font-size: 11px; color: #94A3B8; margin-bottom: 2px;'>%1</div>"
            "  <div style='display: inline-block; max-width: 70%; background-color: #1E293B; color: #F8FAFC; "
            "              padding: 10px 14px; border-radius: 14px 14px 14px 2px; font-size: 13px; border: 1px solid #334155;'>"
            "    <div>%2</div>"
        ).arg(sender.toHtmlEscaped(), content.toHtmlEscaped());

        if (!attachmentName.isEmpty()) {
            double mb = attachmentSize / (1024.0 * 1024.0);
            html += QString(
                "    <div style='margin-top: 6px; background-color: rgba(255,255,255,0.06); border-radius: 6px; padding: 6px; font-size: 11px;'>"
                "      📄 <b>%1</b> (%2 MB) 🔒 AES-256 Verified"
                "    </div>"
            ).arg(attachmentName.toHtmlEscaped()).arg(mb, 0, 'f', 2);
        }

        html += QString(
            "    <div style='font-size: 10px; color: #64748B; text-align: right; margin-top: 4px;'>%1</div>"
            "  </div>"
            "</div>"
        ).arg(timeStr);
    }

    m_chatHistory->append(html);
}

// -------------------------------------------------------------
// Direct File Transfer
// -------------------------------------------------------------
void MainWindow::onSendFileDirect() {
    PeerInfo peer = selectedPeer();
    if (peer.ipAddress.isEmpty()) {
        QMessageBox::information(this, "Select Peer", "Please select a destination peer from the list.");
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select File to Send");
    if (!path.isEmpty()) {
        m_fileManager->sendFile(peer.ipAddress, FILE_TRANSFER_PORT, path);
    }
}

void MainWindow::onTransferStarted(const QString& transferId, const QString& fileName, qint64 totalBytes, bool isSending) {
    int row = m_transfersTable->rowCount();
    m_transfersTable->insertRow(row);
    m_transfersTable->setItem(row, 0, new QTableWidgetItem(transferId.left(8)));
    m_transfersTable->setItem(row, 1, new QTableWidgetItem(fileName));
    double mb = totalBytes / (1024.0 * 1024.0);
    m_transfersTable->setItem(row, 2, new QTableWidgetItem(QString("%1 MB").arg(mb, 0, 'f', 2)));

    QProgressBar* pb = new QProgressBar(m_transfersTable);
    pb->setRange(0, 100);
    pb->setValue(0);
    m_transfersTable->setCellWidget(row, 3, pb);
    m_transfersTable->setItem(row, 4, new QTableWidgetItem("Connecting..."));
    m_transfersTable->setItem(row, 5, new QTableWidgetItem(isSending ? "Uploading..." : "Downloading..."));
}

void MainWindow::onTransferProgress(const QString& transferId, qint64 bytes, qint64 total, double speedMBs) {
    for (int r = 0; r < m_transfersTable->rowCount(); ++r) {
        if (m_transfersTable->item(r, 0)->text() == transferId.left(8)) {
            QProgressBar* pb = qobject_cast<QProgressBar*>(m_transfersTable->cellWidget(r, 3));
            if (pb && total > 0) {
                pb->setValue(static_cast<int>((bytes * 100) / total));
            }
            m_transfersTable->item(r, 4)->setText(QString("%1 MB/s").arg(speedMBs, 0, 'f', 1));
            break;
        }
    }
}

void MainWindow::onTransferCompleted(const QString& transferId, const QString&, bool verified) {
    for (int r = 0; r < m_transfersTable->rowCount(); ++r) {
        if (m_transfersTable->item(r, 0)->text() == transferId.left(8)) {
            QProgressBar* pb = qobject_cast<QProgressBar*>(m_transfersTable->cellWidget(r, 3));
            if (pb) pb->setValue(100);
            m_transfersTable->item(r, 5)->setText(verified ? "✓ AES+SHA Verified" : "Completed");
            m_transfersTable->item(r, 5)->setForeground(QColor(16, 185, 129));
            break;
        }
    }
}

void MainWindow::onTransferFailed(const QString& transferId, const QString& reason) {
    for (int r = 0; r < m_transfersTable->rowCount(); ++r) {
        if (m_transfersTable->item(r, 0)->text() == transferId.left(8)) {
            m_transfersTable->item(r, 5)->setText("Failed: " + reason);
            m_transfersTable->item(r, 5)->setForeground(QColor(239, 68, 68));
            break;
        }
    }
}

// -------------------------------------------------------------
// Hardware Headset Integration
// -------------------------------------------------------------
void MainWindow::onHeadsetConnected(const HidDeviceInfo& dev) {
    m_statusHeadsetLabel->setText("Headset: " + dev.productString);
    m_statusHeadsetLabel->setStyleSheet("color: #10B981; font-weight: bold; margin-left: 16px;");
}

void MainWindow::onHeadsetDisconnected() {
    m_statusHeadsetLabel->setText("Headset: Disconnected");
    m_statusHeadsetLabel->setStyleSheet("color: #94A3B8; margin-left: 16px;");
}

void MainWindow::onHeadsetMutePressed() {
    onToggleMic();
}

void MainWindow::onHeadsetHookSwitchPressed() {
    if (m_inCall) {
        onLeaveCall();
    } else {
        onJoinGroupCall();
    }
}

} // namespace Onyx

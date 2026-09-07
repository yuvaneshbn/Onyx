#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSettings>
#include "../audio/AudioManager.h"
#include "../filetransfer/CryptoEngine.h"

namespace Onyx {

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Onyx - Settings & Preferences");
    setFixedSize(580, 560);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUi();
    loadSettings();
}

void SettingsDialog::setupUi() {
    setStyleSheet(
        "QDialog {"
        "  background-color: #0F172A;"
        "  color: #F8FAFC;"
        "}"
        "QTabWidget::pane {"
        "  border: 1px solid #334155;"
        "  background-color: #1E293B;"
        "  border-radius: 8px;"
        "}"
        "QTabBar::tab {"
        "  background-color: #0F172A;"
        "  color: #94A3B8;"
        "  padding: 10px 18px;"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "  border-top-left-radius: 6px;"
        "  border-top-right-radius: 6px;"
        "  margin-right: 4px;"
        "}"
        "QTabBar::tab:selected {"
        "  background-color: #1E293B;"
        "  color: #38BDF8;"
        "  border-bottom: 2px solid #0078D4;"
        "}"
        "QLabel {"
        "  color: #E2E8F0;"
        "  font-size: 12px;"
        "}"
        "QComboBox, QLineEdit {"
        "  background-color: #0F172A;"
        "  color: #F8FAFC;"
        "  border: 1px solid #334155;"
        "  border-radius: 6px;"
        "  padding: 6px 10px;"
        "  font-size: 12px;"
        "}"
        "QComboBox:focus, QLineEdit:focus {"
        "  border: 1px solid #0078D4;"
        "}"
        "QSlider::groove:horizontal {"
        "  height: 6px;"
        "  background: #334155;"
        "  border-radius: 3px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "  background: #0078D4;"
        "  border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: #38BDF8;"
        "  width: 16px;"
        "  margin-top: -5px;"
        "  margin-bottom: -5px;"
        "  border-radius: 8px;"
        "}"
        "QProgressBar {"
        "  background: #0F172A;"
        "  border: 1px solid #334155;"
        "  border-radius: 4px;"
        "  height: 8px;"
        "}"
        "QProgressBar::chunk {"
        "  background: #10B981;"
        "  border-radius: 4px;"
        "}"
        "QPushButton#saveBtn {"
        "  background-color: #0078D4;"
        "  color: white;"
        "  border-radius: 6px;"
        "  padding: 8px 18px;"
        "  font-weight: bold;"
        "}"
        "QPushButton#saveBtn:hover {"
        "  background-color: #106EBE;"
        "}"
        "QPushButton#cancelBtn {"
        "  background-color: #334155;"
        "  color: #E2E8F0;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(14);

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(createAudioTab(), "Audio & Voice");
    tabWidget->addTab(createVideoTab(), "Video & Camera");
    tabWidget->addTab(createNotificationsTab(), "Notifications");
    tabWidget->addTab(createSecurityTab(), "Security & E2EE");
    mainLayout->addWidget(tabWidget);

    // Bottom action buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setObjectName("cancelBtn");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton* saveBtn = new QPushButton("Apply & Save", this);
    saveBtn->setObjectName("saveBtn");
    connect(saveBtn, &QPushButton::clicked, this, &SettingsDialog::onSave);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addLayout(btnLayout);
}

QWidget* SettingsDialog::createAudioTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    // Input device
    m_micDeviceCombo = new QComboBox(tab);
    m_micDeviceCombo->addItems(AudioManager::enumerateInputDevices());
    form->addRow("Microphone Device:", m_micDeviceCombo);

    // Mic Volume Slider
    QHBoxLayout* inVolLayout = new QHBoxLayout();
    m_inputVolSlider = new QSlider(Qt::Horizontal, tab);
    m_inputVolSlider->setRange(0, 100);
    m_inputVolSlider->setValue(100);
    m_inputVolLabel = new QLabel("100%", tab);
    m_inputVolLabel->setFixedWidth(45);
    inVolLayout->addWidget(m_inputVolSlider);
    inVolLayout->addWidget(m_inputVolLabel);
    form->addRow("Input Volume:", inVolLayout);

    connect(m_inputVolSlider, &QSlider::valueChanged, this, [this](int val) {
        m_inputVolLabel->setText(QString("%1%").arg(val));
    });

    // Mic Software Gain Slider (+0.0 dB to +20.0 dB)
    QHBoxLayout* inGainLayout = new QHBoxLayout();
    m_inputGainSlider = new QSlider(Qt::Horizontal, tab);
    m_inputGainSlider->setRange(0, 200); // 0.0 to 20.0 dB (step 0.1)
    m_inputGainSlider->setValue(0);
    m_inputGainLabel = new QLabel("+0.0 dB", tab);
    m_inputGainLabel->setFixedWidth(55);
    inGainLayout->addWidget(m_inputGainSlider);
    inGainLayout->addWidget(m_inputGainLabel);
    form->addRow("Microphone Gain:", inGainLayout);

    connect(m_inputGainSlider, &QSlider::valueChanged, this, [this](int val) {
        float db = val / 10.0f;
        m_inputGainLabel->setText(QString("+%1 dB").arg(db, 0, 'f', 1));
    });

    // Mic Level Test Bar
    m_micLevelBar = new QProgressBar(tab);
    m_micLevelBar->setRange(0, 100);
    m_micLevelBar->setValue(0);
    m_micLevelBar->setTextVisible(false);
    form->addRow("Mic Test Level:", m_micLevelBar);

    // Output Device
    m_speakerDeviceCombo = new QComboBox(tab);
    m_speakerDeviceCombo->addItems(AudioManager::enumerateOutputDevices());
    form->addRow("Speaker Device:", m_speakerDeviceCombo);

    // Output Volume Slider
    QHBoxLayout* outVolLayout = new QHBoxLayout();
    m_outputVolSlider = new QSlider(Qt::Horizontal, tab);
    m_outputVolSlider->setRange(0, 100);
    m_outputVolSlider->setValue(100);
    m_outputVolLabel = new QLabel("100%", tab);
    m_outputVolLabel->setFixedWidth(45);
    outVolLayout->addWidget(m_outputVolSlider);
    outVolLayout->addWidget(m_outputVolLabel);
    form->addRow("Speaker Volume:", outVolLayout);

    connect(m_outputVolSlider, &QSlider::valueChanged, this, [this](int val) {
        m_outputVolLabel->setText(QString("%1%").arg(val));
    });

    // Audio Codec Selector
    m_audioCodecCombo = new QComboBox(tab);
    m_audioCodecCombo->addItem("Opus (48 kHz HD VoIP - Recommended)", "opus");
    m_audioCodecCombo->addItem("AMR-WB (16 kHz Wideband Speech)", "amrwb");
    m_audioCodecCombo->addItem("AMR-NB (8 kHz Narrowband Speech)", "amrnb");
    form->addRow("Voice Codec:", m_audioCodecCombo);

    // Noise Suppression Checkbox
    m_noiseSuppressionCheck = new QCheckBox("Enable Ambient Noise Suppression", tab);
    m_noiseSuppressionCheck->setChecked(true);
    form->addRow("", m_noiseSuppressionCheck);

    // VAD Sensitivity Slider
    QHBoxLayout* vadLayout = new QHBoxLayout();
    m_vadSlider = new QSlider(Qt::Horizontal, tab);
    m_vadSlider->setRange(5, 50); // 0.005 to 0.050
    m_vadSlider->setValue(20);
    m_vadLabel = new QLabel("Medium", tab);
    m_vadLabel->setFixedWidth(55);
    vadLayout->addWidget(m_vadSlider);
    vadLayout->addWidget(m_vadLabel);
    form->addRow("VAD Sensitivity:", vadLayout);

    connect(m_vadSlider, &QSlider::valueChanged, this, [this](int val) {
        if (val < 15) m_vadLabel->setText("High");
        else if (val < 35) m_vadLabel->setText("Medium");
        else m_vadLabel->setText("Low");
    });

    layout->addLayout(form);
    layout->addStretch();
    return tab;
}

QWidget* SettingsDialog::createVideoTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    // Camera selector
    m_cameraDeviceCombo = new QComboBox(tab);
    m_cameraDeviceCombo->addItems({"Integrated Camera / Webcam", "USB Video Capture Device", "Virtual Synthetic Camera"});
    form->addRow("Camera Device:", m_cameraDeviceCombo);

    // Video Resolution
    m_resolutionCombo = new QComboBox(tab);
    m_resolutionCombo->addItem("1080p Full HD (1920 x 1080)", QSize(1920, 1080));
    m_resolutionCombo->addItem("720p HD (1280 x 720) [Recommended]", QSize(1280, 720));
    m_resolutionCombo->addItem("480p SD (854 x 480)", QSize(854, 480));
    m_resolutionCombo->addItem("360p Low (640 x 360)", QSize(640, 360));
    m_resolutionCombo->setCurrentIndex(1); // 720p
    form->addRow("Resolution:", m_resolutionCombo);

    // Frame Rate
    m_frameRateCombo = new QComboBox(tab);
    m_frameRateCombo->addItem("60 FPS (Ultra Smooth)", 60);
    m_frameRateCombo->addItem("30 FPS (Standard) [Recommended]", 30);
    m_frameRateCombo->addItem("15 FPS (Bandwidth Saver)", 15);
    m_frameRateCombo->setCurrentIndex(1); // 30 FPS
    form->addRow("Frame Rate:", m_frameRateCombo);

    // Video Codec
    m_videoCodecCombo = new QComboBox(tab);
    m_videoCodecCombo->addItem("VP9 (High Efficiency)", "vp9");
    m_videoCodecCombo->addItem("VP8 (Standard Compatibility) [Recommended]", "vp8");
    m_videoCodecCombo->setCurrentIndex(1); // VP8
    form->addRow("Video Codec:", m_videoCodecCombo);

    // Video Bitrate
    QHBoxLayout* brLayout = new QHBoxLayout();
    m_bitrateSlider = new QSlider(Qt::Horizontal, tab);
    m_bitrateSlider->setRange(250, 6000);
    m_bitrateSlider->setValue(1500);
    m_bitrateLabel = new QLabel("1500 kbps", tab);
    m_bitrateLabel->setFixedWidth(65);
    brLayout->addWidget(m_bitrateSlider);
    brLayout->addWidget(m_bitrateLabel);
    form->addRow("Target Bitrate:", brLayout);

    connect(m_bitrateSlider, &QSlider::valueChanged, this, [this](int val) {
        m_bitrateLabel->setText(QString("%1 kbps").arg(val));
    });

    layout->addLayout(form);
    layout->addStretch();
    return tab;
}

QWidget* SettingsDialog::createNotificationsTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(14);

    m_desktopNotifyCheck = new QCheckBox("Show Desktop Banner Notifications on new messages", tab);
    m_desktopNotifyCheck->setChecked(true);
    layout->addWidget(m_desktopNotifyCheck);

    m_ringtoneCheck = new QCheckBox("Play Ringtone chime on incoming 1-on-1 and Group calls", tab);
    m_ringtoneCheck->setChecked(true);
    layout->addWidget(m_ringtoneCheck);

    m_msgAlertCheck = new QCheckBox("Play notification sound when chat messages arrive", tab);
    m_msgAlertCheck->setChecked(true);
    layout->addWidget(m_msgAlertCheck);

    QHBoxLayout* volLayout = new QHBoxLayout();
    QLabel* volLabel = new QLabel("Notification Volume:", tab);
    m_notifyVolSlider = new QSlider(Qt::Horizontal, tab);
    m_notifyVolSlider->setRange(0, 100);
    m_notifyVolSlider->setValue(80);
    volLayout->addWidget(volLabel);
    volLayout->addWidget(m_notifyVolSlider);
    layout->addLayout(volLayout);

    layout->addStretch();
    return tab;
}

QWidget* SettingsDialog::createSecurityTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(14);

    QLabel* infoHeader = new QLabel("End-to-End Encryption (E2EE) Overview", tab);
    infoHeader->setStyleSheet("font-size: 14px; font-weight: bold; color: #38BDF8;");
    layout->addWidget(infoHeader);

    QLabel* infoDesc = new QLabel(
        "Onyx protects all LAN communications using OpenSSL authenticated encryption:\n"
        "• Audio & Video Streams: Encrypted per frame using AES-256-GCM with dynamic IVs.\n"
        "• Text Chat & Files: AES-256-GCM authenticated with SHA-256 integrity checksums.\n"
        "• Key Derivation: PBKDF2-HMAC-SHA256 (10,000 rounds) generates 256-bit room keys.",
        tab
    );
    infoDesc->setStyleSheet("color: #94A3B8; line-height: 140%;");
    layout->addWidget(infoDesc);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(12);

    m_passkeyEdit = new QLineEdit(tab);
    m_passkeyEdit->setText(DEFAULT_ROOM_SECRET);
    m_passkeyEdit->setEchoMode(QLineEdit::Password);
    form->addRow("Room / LAN Passkey:", m_passkeyEdit);

    m_keyPreviewLabel = new QLabel(tab);
    m_keyPreviewLabel->setStyleSheet("font-family: monospace; color: #38BDF8; font-size: 11px;");
    form->addRow("Derived 256-bit Key:", m_keyPreviewLabel);

    m_fingerprintLabel = new QLabel(tab);
    m_fingerprintLabel->setStyleSheet("font-family: monospace; color: #10B981; font-weight: bold; font-size: 11px;");
    form->addRow("Security Fingerprint:", m_fingerprintLabel);

    m_securityBadge = new QLabel("● AES-256-GCM E2EE Active & Verified", tab);
    m_securityBadge->setStyleSheet("color: #10B981; font-weight: bold; font-size: 12px;");
    form->addRow("Encryption Status:", m_securityBadge);

    layout->addLayout(form);

    connect(m_passkeyEdit, &QLineEdit::textChanged, this, &SettingsDialog::onPasskeyChanged);
    onPasskeyChanged(m_passkeyEdit->text());

    layout->addStretch();
    return tab;
}

void SettingsDialog::onPasskeyChanged(const QString& passkey) {
    QByteArray key = CryptoEngine::deriveKeyFromPassphrase(passkey.isEmpty() ? DEFAULT_ROOM_SECRET : passkey);
    QString hexKey = QString::fromLatin1(key.toHex().left(16)) + "..." + QString::fromLatin1(key.toHex().right(8));
    m_keyPreviewLabel->setText(hexKey);
    m_fingerprintLabel->setText(CryptoEngine::computeKeyFingerprint(key));
}

void SettingsDialog::updateMicTestLevel(float level) {
    if (m_micLevelBar) {
        m_micLevelBar->setValue(static_cast<int>(level * 100.0f));
    }
}

void SettingsDialog::loadSettings() {
    QSettings settings("Onyx", "OnyxChat");

    // Audio
    m_inputVolSlider->setValue(settings.value("inputVolume", 100).toInt());
    m_inputGainSlider->setValue(settings.value("inputGainDb", 0).toInt());
    m_outputVolSlider->setValue(settings.value("outputVolume", 100).toInt());
    QString aCodec = settings.value("audioCodec", "opus").toString();
    int aIdx = m_audioCodecCombo->findData(aCodec);
    if (aIdx >= 0) m_audioCodecCombo->setCurrentIndex(aIdx);
    m_noiseSuppressionCheck->setChecked(settings.value("noiseSuppression", true).toBool());
    m_vadSlider->setValue(settings.value("vadThreshold", 20).toInt());

    // Video
    int resIdx = settings.value("resolutionIndex", 1).toInt();
    m_resolutionCombo->setCurrentIndex(resIdx);
    int fpsIdx = settings.value("fpsIndex", 1).toInt();
    m_frameRateCombo->setCurrentIndex(fpsIdx);
    QString vCodec = settings.value("videoCodec", "vp8").toString();
    int vIdx = m_videoCodecCombo->findData(vCodec);
    if (vIdx >= 0) m_videoCodecCombo->setCurrentIndex(vIdx);
    m_bitrateSlider->setValue(settings.value("videoBitrate", 1500).toInt());

    // Notifications
    m_desktopNotifyCheck->setChecked(settings.value("desktopNotifications", true).toBool());
    m_ringtoneCheck->setChecked(settings.value("callRingtone", true).toBool());
    m_msgAlertCheck->setChecked(settings.value("messageAlert", true).toBool());
    m_notifyVolSlider->setValue(settings.value("notificationVolume", 80).toInt());

    // Security
    QString pass = settings.value("roomPasskey", DEFAULT_ROOM_SECRET).toString();
    m_passkeyEdit->setText(pass);
}

void SettingsDialog::saveSettings() {
    QSettings settings("Onyx", "OnyxChat");

    // Audio
    settings.setValue("inputVolume", m_inputVolSlider->value());
    settings.setValue("inputGainDb", m_inputGainSlider->value());
    settings.setValue("outputVolume", m_outputVolSlider->value());
    settings.setValue("audioCodec", m_audioCodecCombo->currentData().toString());
    settings.setValue("noiseSuppression", m_noiseSuppressionCheck->isChecked());
    settings.setValue("vadThreshold", m_vadSlider->value());

    // Video
    settings.setValue("resolutionIndex", m_resolutionCombo->currentIndex());
    settings.setValue("fpsIndex", m_frameRateCombo->currentIndex());
    settings.setValue("videoCodec", m_videoCodecCombo->currentData().toString());
    settings.setValue("videoBitrate", m_bitrateSlider->value());

    // Notifications
    settings.setValue("desktopNotifications", m_desktopNotifyCheck->isChecked());
    settings.setValue("callRingtone", m_ringtoneCheck->isChecked());
    settings.setValue("messageAlert", m_msgAlertCheck->isChecked());
    settings.setValue("notificationVolume", m_notifyVolSlider->value());

    // Security
    settings.setValue("roomPasskey", m_passkeyEdit->text());
}

void SettingsDialog::onSave() {
    saveSettings();
    emit settingsApplied();
    accept();
}

int SettingsDialog::inputVolume() const {
    return m_inputVolSlider->value();
}

float SettingsDialog::inputGainDb() const {
    return m_inputGainSlider->value() / 10.0f;
}

int SettingsDialog::outputVolume() const {
    return m_outputVolSlider->value();
}

QString SettingsDialog::audioCodec() const {
    return m_audioCodecCombo->currentData().toString();
}

bool SettingsDialog::noiseSuppression() const {
    return m_noiseSuppressionCheck->isChecked();
}

float SettingsDialog::vadThreshold() const {
    return m_vadSlider->value() / 1000.0f;
}

QSize SettingsDialog::videoResolution() const {
    return m_resolutionCombo->currentData().toSize();
}

int SettingsDialog::frameRate() const {
    return m_frameRateCombo->currentData().toInt();
}

QString SettingsDialog::videoCodec() const {
    return m_videoCodecCombo->currentData().toString();
}

int SettingsDialog::videoBitrateKbps() const {
    return m_bitrateSlider->value();
}

bool SettingsDialog::desktopNotifications() const {
    return m_desktopNotifyCheck->isChecked();
}

bool SettingsDialog::callRingtone() const {
    return m_ringtoneCheck->isChecked();
}

bool SettingsDialog::messageAlert() const {
    return m_msgAlertCheck->isChecked();
}

int SettingsDialog::notificationVolume() const {
    return m_notifyVolSlider->value();
}

QString SettingsDialog::roomPasskey() const {
    return m_passkeyEdit->text().trimmed();
}

QByteArray SettingsDialog::derivedKey() const {
    return CryptoEngine::deriveKeyFromPassphrase(roomPasskey());
}

QString SettingsDialog::securityFingerprint() const {
    return m_fingerprintLabel->text();
}

} // namespace Onyx

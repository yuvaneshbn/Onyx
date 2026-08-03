#include "SettingsDlg.h"
#include "mainDlg.h"
#include "MessagesDlg.h"
#include "Dialer.h"
#include "settings.h"
#include "global.h"
#include "Preview.h"
#include "langpack.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>

// Keep the same static data
static QString defaultActionItems[] = {
    "",
    "call",
#ifdef _GLOBAL_VIDEO
    "video",
#endif
    "message"
};
static QString defaultActionValues[] = {
    "Default",
    "Call",
#ifdef _GLOBAL_VIDEO
    "Video Call",
#endif
    "Message"
};

SettingsDlg::SettingsDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi();
    translateUi();
    initDialog();
    // The original showed the window immediately; we do it in the caller.
}

SettingsDlg::~SettingsDlg() {}

// ---------- UI construction (keeps the same logical layout) ----------
void SettingsDlg::setupUi()
{
    setWindowTitle("Settings");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // --- Ringtone & Volume ---
    QGroupBox *groupRing = new QGroupBox(Translate("Ringtone"));
    QGridLayout *ringLayout = new QGridLayout;
    m_ringtone = new QLineEdit; m_ringtone->setObjectName("ringtone");
    m_btnBrowseRing = new QPushButton("...");
    m_btnDefaultRing = new QPushButton(Translate("Default"));
    m_volumeRing = new QSlider(Qt::Horizontal); m_volumeRing->setRange(0, 100);
    ringLayout->addWidget(new QLabel(Translate("File:")), 0, 0);
    ringLayout->addWidget(m_ringtone, 0, 1, 1, 2);
    ringLayout->addWidget(m_btnBrowseRing, 0, 3);
    ringLayout->addWidget(m_btnDefaultRing, 0, 4);
    ringLayout->addWidget(new QLabel(Translate("Volume:")), 1, 0);
    ringLayout->addWidget(m_volumeRing, 1, 1, 1, 4);
    groupRing->setLayout(ringLayout);
    mainLayout->addWidget(groupRing);

    // --- Audio Devices ---
    QGroupBox *groupDev = new QGroupBox(Translate("Audio Devices"));
    QFormLayout *devLayout = new QFormLayout;
    m_audioInput = new QComboBox; m_audioInput->setObjectName("audioInput");
    m_audioOutput = new QComboBox; m_audioOutput->setObjectName("audioOutput");
    m_audioRing = new QComboBox; m_audioRing->setObjectName("audioRing");
    devLayout->addRow(Translate("Microphone:"), m_audioInput);
    devLayout->addRow(Translate("Speakers:"), m_audioOutput);
    devLayout->addRow(Translate("Ring device:"), m_audioRing);
    groupDev->setLayout(devLayout);
    mainLayout->addWidget(groupDev);

    // --- Audio Options (checkboxes) ---
    QGroupBox *groupAudioOpt = new QGroupBox(Translate("Audio Options"));
    QGridLayout *audioOptLayout = new QGridLayout;
    m_micAmplif = new QCheckBox(Translate("Microphone amplification"));
    m_swAdjust = new QCheckBox(Translate("Software level adjustment"));
    m_vad = new QCheckBox(Translate("Voice Activity Detection"));
    m_ec = new QCheckBox(Translate("Echo Canceller"));
    m_opusStereo = new QCheckBox(Translate("Opus stereo"));
    m_forceCodec = new QCheckBox(Translate("Force codec"));
    audioOptLayout->addWidget(m_micAmplif, 0, 0);
    audioOptLayout->addWidget(m_swAdjust, 0, 1);
    audioOptLayout->addWidget(m_vad, 1, 0);
    audioOptLayout->addWidget(m_ec, 1, 1);
    audioOptLayout->addWidget(m_opusStereo, 2, 0);
    audioOptLayout->addWidget(m_forceCodec, 2, 1);
    groupAudioOpt->setLayout(audioOptLayout);
    mainLayout->addWidget(groupAudioOpt);

    // --- Codec Lists ---
    QGroupBox *groupCodecs = new QGroupBox(Translate("Audio Codecs"));
    QHBoxLayout *codecLayout = new QHBoxLayout;
    m_codecsAll = new QListWidget; m_codecsAll->setObjectName("codecsAll");
    m_codecsEnabled = new QListWidget; m_codecsEnabled->setObjectName("codecsEnabled");
    // Buttons to move items (simulate spin modify/order)
    QVBoxLayout *btnCodecLayout = new QVBoxLayout;
    QPushButton *btnAdd = new QPushButton(">>");
    QPushButton *btnRemove = new QPushButton("<<");
    QPushButton *btnUp = new QPushButton(Translate("Up"));
    QPushButton *btnDown = new QPushButton(Translate("Down"));
    btnCodecLayout->addStretch();
    btnCodecLayout->addWidget(btnAdd);
    btnCodecLayout->addWidget(btnRemove);
    btnCodecLayout->addWidget(btnUp);
    btnCodecLayout->addWidget(btnDown);
    btnCodecLayout->addStretch();
    codecLayout->addWidget(m_codecsAll);
    codecLayout->addLayout(btnCodecLayout);
    codecLayout->addWidget(m_codecsEnabled);
    groupCodecs->setLayout(codecLayout);
    mainLayout->addWidget(groupCodecs);
    // Connect codec move buttons
    connect(btnAdd, &QPushButton::clicked, this, &SettingsDlg::onDeltaposSpinModifyAdd);
    connect(btnRemove, &QPushButton::clicked, this, &SettingsDlg::onDeltaposSpinModifyRemove);
    connect(btnUp, &QPushButton::clicked, this, &SettingsDlg::onDeltaposSpinOrderUp);
    connect(btnDown, &QPushButton::clicked, this, &SettingsDlg::onDeltaposSpinOrderDown);

    // --- DTMF, Auto Answer, Forwarding, Deny Incoming, Directory ---
    QGroupBox *groupCallOpt = new QGroupBox(Translate("Call Options"));
    QFormLayout *callOptLayout = new QFormLayout;
    m_dtmfMethod = new QComboBox;
    m_autoAnswer = new QComboBox;
    m_forwarding = new QComboBox;
    m_fwdNumber = new QLineEdit;
    m_fwdDelay = new QLineEdit;
    m_denyIncoming = new QComboBox;
    m_directory = new QLineEdit;
    m_defaultAction = new QComboBox;
    callOptLayout->addRow(Translate("DTMF method:"), m_dtmfMethod);
    callOptLayout->addRow(Translate("Auto answer:"), m_autoAnswer);
    callOptLayout->addRow(Translate("Forwarding:"), m_forwarding);
    callOptLayout->addRow(Translate("Forward number:"), m_fwdNumber);
    callOptLayout->addRow(Translate("Forward delay:"), m_fwdDelay);
    callOptLayout->addRow(Translate("Deny incoming:"), m_denyIncoming);
    callOptLayout->addRow(Translate("Directory:"), m_directory);
    callOptLayout->addRow(Translate("Default action:"), m_defaultAction);
    groupCallOpt->setLayout(callOptLayout);
    mainLayout->addWidget(groupCallOpt);

    // --- Checkboxes (grouped) ---
    QGroupBox *groupGeneral = new QGroupBox(Translate("General"));
    QGridLayout *generalLayout = new QGridLayout;
    m_singleMode = new QCheckBox(Translate("Single mode"));
    m_enableLog = new QCheckBox(Translate("Enable log"));
    m_bringToFront = new QCheckBox(Translate("Bring to front on incoming"));
    m_randomAnswer = new QCheckBox(Translate("Random answer box"));
    m_callWaiting = new QCheckBox(Translate("Call waiting"));
    m_multiMonitor = new QCheckBox(Translate("Multi-monitor"));
    m_networkChanges = new QCheckBox(Translate("Network changes"));
    m_disableMessaging = new QCheckBox(Translate("Disable messaging"));
    m_disableNameLookup = new QCheckBox(Translate("Disable name lookup"));
    m_enableLocal = new QCheckBox(Translate("Enable local account"));
    m_mediaButtons = new QCheckBox(Translate("Handle media buttons"));
    m_headset = new QCheckBox(Translate("Headset support"));
    m_localDtmf = new QCheckBox(Translate("Local DTMF"));
    int row = 0;
    generalLayout->addWidget(m_singleMode, row, 0); ++row;
    generalLayout->addWidget(m_enableLog, row, 0); ++row;
    generalLayout->addWidget(m_bringToFront, row, 0); ++row;
    generalLayout->addWidget(m_randomAnswer, row, 0); ++row;
    generalLayout->addWidget(m_callWaiting, row, 0); ++row;
    generalLayout->addWidget(m_multiMonitor, row, 0); ++row;
    generalLayout->addWidget(m_networkChanges, row, 0); ++row;
    generalLayout->addWidget(m_disableMessaging, row, 0); ++row;
    generalLayout->addWidget(m_disableNameLookup, row, 0); ++row;
    generalLayout->addWidget(m_enableLocal, row, 0); ++row;
    generalLayout->addWidget(m_mediaButtons, row, 0); ++row;
    generalLayout->addWidget(m_headset, row, 0); ++row;
    generalLayout->addWidget(m_localDtmf, row, 0); ++row;
    groupGeneral->setLayout(generalLayout);
    mainLayout->addWidget(groupGeneral);

    // Startup & updates
    QGroupBox *groupStartup = new QGroupBox(Translate("Startup & Updates"));
    QFormLayout *startupLayout = new QFormLayout;
    m_startup = new QCheckBox(Translate("Run at system startup"));
    m_updatesInterval = new QComboBox;
    startupLayout->addRow(m_startup);
    startupLayout->addRow(Translate("Check for updates:"), m_updatesInterval);
    groupStartup->setLayout(startupLayout);
    mainLayout->addWidget(groupStartup);

#ifdef _GLOBAL_VIDEO
    // Video section
    QGroupBox *groupVideo = new QGroupBox(Translate("Video"));
    QFormLayout *videoLayout = new QFormLayout;
    m_disableVideo = new QCheckBox(Translate("Disable video"));
    m_vidCapDev = new QComboBox;
    m_videoCodec = new QComboBox;
    m_videoH264 = new QCheckBox("H264");
    m_videoH263 = new QCheckBox("H263");
    m_videoVP8 = new QCheckBox("VP8");
    m_videoVP9 = new QCheckBox("VP9");
    m_videoBitrate = new QLineEdit;
    m_previewBtn = new QPushButton(Translate("Preview"));
    videoLayout->addRow(m_disableVideo);
    videoLayout->addRow(Translate("Capture device:"), m_vidCapDev);
    videoLayout->addRow(Translate("Codec:"), m_videoCodec);
    QHBoxLayout *vidCodecLayout = new QHBoxLayout;
    vidCodecLayout->addWidget(m_videoH264);
    vidCodecLayout->addWidget(m_videoH263);
    vidCodecLayout->addWidget(m_videoVP8);
    vidCodecLayout->addWidget(m_videoVP9);
    videoLayout->addRow(Translate("Enable codecs:"), vidCodecLayout);
    videoLayout->addRow(Translate("Bitrate (kbps):"), m_videoBitrate);
    videoLayout->addRow(m_previewBtn);
    groupVideo->setLayout(videoLayout);
    mainLayout->addWidget(groupVideo);
    connect(m_previewBtn, &QPushButton::clicked, this, &SettingsDlg::onBnClickedPreview);
#endif

    // Recording
    QGroupBox *groupRec = new QGroupBox(Translate("Recording"));
    QGridLayout *recLayout = new QGridLayout;
    m_recordingPath = new QLineEdit;
    m_btnRecBrowse = new QPushButton("...");
    m_btnRecDefault = new QPushButton(Translate("Default"));
    m_recMp3 = new QRadioButton("MP3");
    m_recWav = new QRadioButton("WAV");
    m_recAuto = new QCheckBox(Translate("Auto record"));
    m_recButton = new QCheckBox(Translate("Show record button"));
    recLayout->addWidget(new QLabel(Translate("Path:")), 0, 0);
    recLayout->addWidget(m_recordingPath, 0, 1, 1, 2);
    recLayout->addWidget(m_btnRecBrowse, 0, 3);
    recLayout->addWidget(m_btnRecDefault, 0, 4);
    QHBoxLayout *recFormatLayout = new QHBoxLayout;
    recFormatLayout->addWidget(m_recMp3);
    recFormatLayout->addWidget(m_recWav);
    recLayout->addWidget(new QLabel(Translate("Format:")), 1, 0);
    recLayout->addLayout(recFormatLayout, 1, 1, 1, 4);
    recLayout->addWidget(m_recAuto, 2, 0, 1, 2);
    recLayout->addWidget(m_recButton, 2, 2, 1, 3);
    groupRec->setLayout(recLayout);
    mainLayout->addWidget(groupRec);

    // Ports and network
    QGroupBox *groupNet = new QGroupBox(Translate("Network"));
    QFormLayout *netLayout = new QFormLayout;
    m_rport = new QCheckBox(Translate("Symmetric RTP"));
    m_sourcePort = new QLineEdit;
    m_rtpPortMin = new QLineEdit;
    m_rtpPortMax = new QLineEdit;
    m_dnsSrvNs = new QLineEdit;
    m_dnsSrvCheckbox = new QCheckBox(Translate("Enable DNS SRV"));
    m_stun = new QLineEdit;
    m_stunCheckbox = new QCheckBox(Translate("Enable STUN"));
    netLayout->addRow(m_rport);
    netLayout->addRow(Translate("Source port:"), m_sourcePort);
    netLayout->addRow(Translate("RTP port min:"), m_rtpPortMin);
    netLayout->addRow(Translate("RTP port max:"), m_rtpPortMax);
    netLayout->addRow(Translate("DNS SRV nameserver:"), m_dnsSrvNs);
    netLayout->addRow(m_dnsSrvCheckbox);
    netLayout->addRow(Translate("STUN server:"), m_stun);
    netLayout->addRow(m_stunCheckbox);
    groupNet->setLayout(netLayout);
    mainLayout->addWidget(groupNet);

    // Feature Codes & AA Options buttons
    QHBoxLayout *linkLayout = new QHBoxLayout;
    m_btnFeatureCodes = new QPushButton(Translate("Feature Codes"));
    m_btnAAOptions = new QPushButton(Translate("AA Options"));
    linkLayout->addWidget(m_btnFeatureCodes);
    linkLayout->addWidget(m_btnAAOptions);
    linkLayout->addStretch();
    mainLayout->addLayout(linkLayout);

    // OK / Cancel
    QHBoxLayout *dialogBtnLayout = new QHBoxLayout;
    dialogBtnLayout->addStretch();
    QPushButton *okBtn = new QPushButton(Translate("OK"));
    QPushButton *cancelBtn = new QPushButton(Translate("Cancel"));
    dialogBtnLayout->addWidget(okBtn);
    dialogBtnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(dialogBtnLayout);

    // Connect main actions
    connect(m_btnBrowseRing, &QPushButton::clicked, this, &SettingsDlg::onBnClickedBrowse);
    connect(m_btnDefaultRing, &QPushButton::clicked, this, &SettingsDlg::onBnClickedDefault);
    connect(m_ringtone, &QLineEdit::textChanged, this, &SettingsDlg::onChangeRingtone);
    connect(m_volumeRing, &QSlider::sliderReleased, this, &SettingsDlg::onVolumeRingChanged);
    connect(m_btnRecBrowse, &QPushButton::clicked, this, &SettingsDlg::onBnClickedRecordingBrowse);
    connect(m_recordingPath, &QLineEdit::textChanged, this, &SettingsDlg::onEnChangeRecording);
    connect(m_btnRecDefault, &QPushButton::clicked, this, &SettingsDlg::onBnClickedRecordingDefault);
    connect(m_btnFeatureCodes, &QPushButton::clicked, this, &SettingsDlg::onFeatureCodesClicked);
    connect(m_btnAAOptions, &QPushButton::clicked, this, &SettingsDlg::onBnClickedAAOptions);
    connect(m_dnsSrvCheckbox, &QCheckBox::toggled, this, &SettingsDlg::onBnClickedDnsSrv);
    connect(m_stunCheckbox, &QCheckBox::toggled, this, &SettingsDlg::onBnClickedStun);
    connect(okBtn, &QPushButton::clicked, this, &SettingsDlg::onBnClickedOk);
    connect(cancelBtn, &QPushButton::clicked, this, &SettingsDlg::onBnClickedCancel);

    // Install event filter for VKEYTOITEM (space/delete in list widgets)
    m_codecsAll->installEventFilter(this);
    m_codecsEnabled->installEventFilter(this);
}

void SettingsDlg::translateUi()
{
    setWindowTitle(Translate("Settings"));
    // All strings are already wrapped with Translate() where needed above.
    // Additional dynamic translation can be done here if required.
}

// ---------- OnInitDialog equivalent (called after construction, before show) ----------
void SettingsDlg::populateAudioDevices()
{
    m_audioInput->addItem(Translate("Default"));
    m_audioOutput->addItem(Translate("Default"));
    m_audioRing->addItem(Translate("Default"));
    m_audioInput->setCurrentIndex(0);
    m_audioOutput->setCurrentIndex(0);
    m_audioRing->setCurrentIndex(0);

    pjmedia_aud_dev_info aud_dev_info[PJMEDIA_AUD_MAX_DEVS];
    unsigned count = 0;
    if (is_pjsua_running()) {
        count = PJMEDIA_AUD_MAX_DEVS;
        pjsua_enum_aud_devs(aud_dev_info, &count);
    }
    for (unsigned i = 0; i < count; i++) {
        QString name = Onyx::Utf8DecodeUni(aud_dev_info[i].name);
        if (aud_dev_info[i].input_count) {
            m_audioInput->addItem(name);
            if (accountSettings.audioInputDevice == name)
                m_audioInput->setCurrentIndex(m_audioInput->count() - 1);
        }
        if (aud_dev_info[i].output_count) {
            m_audioOutput->addItem(name);
            m_audioRing->addItem(name);
            if (accountSettings.audioOutputDevice == name)
                m_audioOutput->setCurrentIndex(m_audioOutput->count() - 1);
            if (accountSettings.audioRingDevice == name)
                m_audioRing->setCurrentIndex(m_audioRing->count() - 1);
        }
    }
}

void SettingsDlg::populateCodecLists()
{
    m_codecsAll->clear();
    m_codecsEnabled->clear();

    pjsua_codec_info codec_info[PJMEDIA_CODEC_MGR_MAX_CODECS];
    unsigned count = 0;
    if (is_pjsua_running()) {
        count = PJMEDIA_CODEC_MGR_MAX_CODECS;
        pjsua_enum_codecs(codec_info, &count);
    }

    // Build a set of disabled codec keys
    QSet<QString> disabledKeys;
    for (unsigned i = 0; i < count; i++) {
        QString key = Onyx::PjToStr(&codec_info[i].codec_id);
        QString value = mainDlg->audioCodecList.value(key);
        if (codec_info[i].priority &&
            (!accountSettings.audioCodecs.isEmpty() || QStringLiteral(_GLOBAL_CODECS_ENABLED).contains(key))) {
            // Enabled codec
            if (m_codecsEnabled->findItems(value, Qt::MatchExactly).isEmpty())
                m_codecsEnabled->addItem(value);
        } else {
            disabledKeys.insert(key);
        }
    }
    // Add all codecs that are disabled to the left list
    QMapIterator<QString, QString> it(mainDlg->audioCodecList);
    while (it.hasNext()) {
        it.next();
        if (disabledKeys.contains(it.key())) {
            m_codecsAll->addItem(it.value());
        }
    }
}

// Public slot to be called after construction to initialise all values
void SettingsDlg::initDialog()
{
    // Ringtone & volume
    m_ringtone->setText(accountSettings.ringtone);
    m_volumeRing->setValue(accountSettings.volumeRing);

    // Recording
    m_recordingPath->setText(accountSettings.recordingPath);
    if (accountSettings.recordingFormat == "wav") {
        m_recWav->setChecked(true);
    } else {
        m_recMp3->setChecked(true);
    }
    m_recAuto->setChecked(accountSettings.autoRecording);
    m_recButton->setChecked(accountSettings.recordingButton);

    // Audio devices
    populateAudioDevices();

    // Audio options
    m_micAmplif->setChecked(accountSettings.micAmplification);
    m_swAdjust->setChecked(accountSettings.swLevelAdjustment);
    m_vad->setChecked(accountSettings.vad);
    m_ec->setChecked(accountSettings.ec);
    m_opusStereo->setChecked(accountSettings.opusStereo);
    m_forceCodec->setChecked(accountSettings.forceCodec);

    // Codecs
    populateCodecLists();

#ifdef _GLOBAL_VIDEO
    m_disableVideo->setChecked(accountSettings.disableVideo);
    m_vidCapDev->addItem(Translate("Default"));
    m_vidCapDev->setCurrentIndex(0);
    pjmedia_vid_dev_info vid_dev_info[PJMEDIA_VID_DEV_MAX_DEVS];
    unsigned vidCount = 0;
    if (is_pjsua_running()) {
        vidCount = PJMEDIA_VID_DEV_MAX_DEVS;
        pjsua_vid_enum_devs(vid_dev_info, &vidCount);
    }
    for (unsigned i = 0; i < vidCount; i++) {
        if (vid_dev_info[i].fmt_cnt &&
            (vid_dev_info[i].dir == PJMEDIA_DIR_ENCODING ||
             vid_dev_info[i].dir == PJMEDIA_DIR_ENCODING_DECODING)) {
            QString name = Onyx::Utf8DecodeUni(vid_dev_info[i].name);
            m_vidCapDev->addItem(name);
            if (accountSettings.videoCaptureDevice == name)
                m_vidCapDev->setCurrentIndex(m_vidCapDev->count() - 1);
        }
    }

    m_videoCodec->addItem(Translate("Default"));
    m_videoCodec->setCurrentIndex(0);
    pjsua_codec_info vidCodecInfo[PJMEDIA_CODEC_MGR_MAX_CODECS];
    if (is_pjsua_running()) {
        vidCount = PJMEDIA_CODEC_MGR_MAX_CODECS;
        pjsua_vid_enum_codecs(vidCodecInfo, &vidCount);
    }
    for (unsigned i = 0; i < vidCount; i++) {
        QString codecId = Onyx::PjToStr(&vidCodecInfo[i].codec_id);
        m_videoCodec->addItem(codecId);
        if (accountSettings.videoCodec == codecId)
            m_videoCodec->setCurrentIndex(m_videoCodec->count() - 1);
    }

    m_videoH264->setChecked(accountSettings.videoH264);
    m_videoH263->setChecked(accountSettings.videoH263);
    m_videoVP8->setChecked(accountSettings.videoVP8);
    m_videoVP9->setChecked(accountSettings.videoVP9);

    if (!accountSettings.videoBitrate) {
        if (is_pjsua_running()) {
            char h264[] = "H264";
            const pj_str_t codec_id = pj_str(h264);
            pjmedia_vid_codec_param param;
            pjsua_vid_codec_get_param(&codec_id, &param);
            accountSettings.videoBitrate = param.enc_fmt.det.vid.max_bps / 1000;
        }
    }
    m_videoBitrate->setText(QString::number(accountSettings.videoBitrate));
#endif

    // Network
    m_rport->setChecked(accountSettings.rport);
    m_sourcePort->setText(QString::number(accountSettings.sourcePort));
    m_rtpPortMin->setText(QString::number(accountSettings.rtpPortMin));
    m_rtpPortMax->setText(QString::number(accountSettings.rtpPortMax));
    m_dnsSrvNs->setText(accountSettings.dnsSrvNs);
    m_dnsSrvCheckbox->setChecked(accountSettings.dnsSrv);
    m_stun->setText(accountSettings.stun);
    m_stunCheckbox->setChecked(accountSettings.enableSTUN);

    // DTMF, auto answer, forwarding, deny incoming, directory, default action
    m_dtmfMethod->addItems({Translate("Auto"), Translate("In-band"), Translate("RFC2833"), Translate("SIP-INFO")});
    m_dtmfMethod->setCurrentIndex(accountSettings.DTMFMethod);

    autoAnswerValues << "" << "button" << "header" << "all";
    m_autoAnswer->addItems({Translate("No"), Translate("Control Button"), Translate("SIP Header"), Translate("All Calls")});
    int idx = autoAnswerValues.indexOf(accountSettings.autoAnswer);
    m_autoAnswer->setCurrentIndex(idx >= 0 ? idx : 0);

    forwardingValues << "" << "button" << "all";
    m_forwarding->addItems({Translate("No"), Translate("Control Button"), Translate("All Calls")});
    idx = forwardingValues.indexOf(accountSettings.forwarding);
    m_forwarding->setCurrentIndex(idx >= 0 ? idx : 0);
    m_fwdNumber->setText(accountSettings.forwardingNumber);
    m_fwdDelay->setText(QString::number(accountSettings.forwardingDelay));

    denyIncomingValues << "" << "button" << "user" << "domain" << "userdomain" << "remotedomain" << "all";
    m_denyIncoming->addItems({
        Translate("No"), Translate("Control Button"), Translate("Different User"),
        Translate("Different Domain"), Translate("Different User or Domain"),
        Translate("Different Remote Domain"), Translate("All Calls")
    });
    idx = denyIncomingValues.indexOf(accountSettings.denyIncoming);
    m_denyIncoming->setCurrentIndex(idx >= 0 ? idx : 0);

    m_directory->setText(accountSettings.usersDirectory);

    int n = sizeof(defaultActionItems)/sizeof(defaultActionItems[0]);
    bool found = false;
    for (int i = 0; i < n; i++) {
        m_defaultAction->addItem(Translate(defaultActionValues[i]));
        if (accountSettings.defaultAction == defaultActionItems[i]) {
            m_defaultAction->setCurrentIndex(i);
            found = true;
        }
    }
    if (!found) m_defaultAction->setCurrentIndex(0);

    // General checkboxes
    m_mediaButtons->setChecked(accountSettings.enableMediaButtons);
    m_headset->setChecked(accountSettings.headsetSupport);
    m_localDtmf->setChecked(accountSettings.localDTMF);
    m_singleMode->setChecked(accountSettings.singleMode);
    m_enableLog->setChecked(accountSettings.enableLog);
    m_bringToFront->setChecked(accountSettings.bringToFrontOnIncoming);
    m_randomAnswer->setChecked(accountSettings.randomAnswerBox);
    m_callWaiting->setChecked(accountSettings.callWaiting);
    m_multiMonitor->setChecked(accountSettings.multiMonitor);
    m_networkChanges->setChecked(accountSettings.networkChanges);
    m_disableMessaging->setChecked(accountSettings.disableMessaging);
    m_disableNameLookup->setChecked(accountSettings.disableNameLookup);
    m_enableLocal->setChecked(accountSettings.enableLocalAccount);

    // Updates interval
    m_updatesInterval->addItems({Translate("Daily"), Translate("Weekly"), Translate("Monthly"),
                                 Translate("Quarterly"), Translate("Never")});
    int updIdx = 1; // Weekly default
    if (accountSettings.updatesInterval == "daily") updIdx = 0;
    else if (accountSettings.updatesInterval == "monthly") updIdx = 2;
    else if (accountSettings.updatesInterval == "quarterly") updIdx = 3;
    else if (accountSettings.updatesInterval == "never") updIdx = 4;
    m_updatesInterval->setCurrentIndex(updIdx);

    // Startup check (Windows registry)
    QSettings regRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                     QSettings::NativeFormat);
    QString runValue = regRun.value(_GLOBAL_NAME_NICE).toString();
    if (!runValue.isEmpty() && runValue.toLower().contains(accountSettings.exeFile.toLower())) {
        m_startup->setChecked(true);
    }
}

// ---------- Event filter for VKEYTOITEM (space/delete keys on codec lists) ----------
bool SettingsDlg::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (obj == m_codecsAll && m_codecsAll->currentRow() != -1 && keyEvent->key() == Qt::Key_Space) {
            onDeltaposSpinModifyAdd();
            return true;
        }
        if (obj == m_codecsEnabled && m_codecsEnabled->currentRow() != -1 && keyEvent->key() == Qt::Key_Delete) {
            onDeltaposSpinModifyRemove();
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}

// ---------- Slots (translated 1:1) ----------
void SettingsDlg::onClose() { close(); }

void SettingsDlg::onBnClickedCancel() {
    mainDlg->PlayerStop();
    onClose();
}

void SettingsDlg::onBnClickedOk() {
    hide();
    mainDlg->PJDestroy();
    QTimer::singleShot(0, this, &SettingsDlg::onUpdateSettings);
}

void SettingsDlg::onUpdateSettings() {
    // Audio devices
    accountSettings.audioInputDevice = m_audioInput->currentText();
    if (accountSettings.audioInputDevice == Translate("Default"))
        accountSettings.audioInputDevice.clear();
    accountSettings.audioOutputDevice = m_audioOutput->currentText();
    if (accountSettings.audioOutputDevice == Translate("Default"))
        accountSettings.audioOutputDevice.clear();
    accountSettings.audioRingDevice = m_audioRing->currentText();
    if (accountSettings.audioRingDevice == Translate("Default"))
        accountSettings.audioRingDevice.clear();

    accountSettings.micAmplification = m_micAmplif->isChecked();
    accountSettings.swLevelAdjustment = m_swAdjust->isChecked();
    accountSettings.vad = m_vad->isChecked();
    accountSettings.ec = m_ec->isChecked();
    accountSettings.opusStereo = m_opusStereo->isChecked();
    accountSettings.forceCodec = m_forceCodec->isChecked();

    // Codecs
    bool hasStereo = false;
    accountSettings.audioCodecs.clear();
    for (int i = 0; i < m_codecsEnabled->count(); ++i) {
        QString value = m_codecsEnabled->item(i)->text();
        QString key = mainDlg->audioCodecList.key(value); // reverse lookup
        if (!key.isEmpty()) {
            accountSettings.audioCodecs += key + " ";
            if (!hasStereo && key.endsWith("/2") && !key.startsWith("opus"))
                hasStereo = true;
        }
    }
    accountSettings.audioCodecs = accountSettings.audioCodecs.trimmed();
    if (hasStereo && accountSettings.ec)
        QMessageBox::warning(this, "Warning", "Echo Canceler enabled. Stereo will be converted to Mono.");

#ifdef _GLOBAL_VIDEO
    accountSettings.disableVideo = m_disableVideo->isChecked();
    accountSettings.videoCaptureDevice = m_vidCapDev->currentText();
    if (accountSettings.videoCaptureDevice == Translate("Default"))
        accountSettings.videoCaptureDevice.clear();
    accountSettings.videoCodec = m_videoCodec->currentText();
    if (accountSettings.videoCodec == Translate("Default"))
        accountSettings.videoCodec.clear();
    accountSettings.videoH264 = m_videoH264->isChecked();
    accountSettings.videoH263 = m_videoH263->isChecked();
    accountSettings.videoVP8 = m_videoVP8->isChecked();
    accountSettings.videoVP9 = m_videoVP9->isChecked();
    accountSettings.videoBitrate = m_videoBitrate->text().toInt();
#endif

    accountSettings.rport = m_rport->isChecked();
    accountSettings.sourcePort = m_sourcePort->text().toInt();
    accountSettings.rtpPortMin = m_rtpPortMin->text().toInt();
    accountSettings.rtpPortMax = m_rtpPortMax->text().toInt();

    accountSettings.dnsSrvNs = m_dnsSrvNs->text().trimmed();
    accountSettings.dnsSrv = (!accountSettings.dnsSrvNs.isEmpty() && m_dnsSrvCheckbox->isChecked());
    accountSettings.stun = m_stun->text().trimmed();
    accountSettings.enableSTUN = (!accountSettings.stun.isEmpty() && m_stunCheckbox->isChecked());

    accountSettings.DTMFMethod = m_dtmfMethod->currentIndex();
    accountSettings.autoAnswer = autoAnswerValues.value(m_autoAnswer->currentIndex());
    accountSettings.forwarding = forwardingValues.value(m_forwarding->currentIndex());
    accountSettings.forwardingNumber = m_fwdNumber->text();
    accountSettings.forwardingDelay = m_fwdDelay->text().toInt();
    accountSettings.denyIncoming = denyIncomingValues.value(m_denyIncoming->currentIndex());
    accountSettings.usersDirectory = m_directory->text().trimmed();
    accountSettings.defaultAction = defaultActionItems[m_defaultAction->currentIndex()];
    accountSettings.enableMediaButtons = m_mediaButtons->isChecked();
    accountSettings.headsetSupport = m_headset->isChecked();
    accountSettings.localDTMF = m_localDtmf->isChecked();
    accountSettings.singleMode = m_singleMode->isChecked();
    accountSettings.enableLog = m_enableLog->isChecked();
    accountSettings.bringToFrontOnIncoming = m_bringToFront->isChecked();
    accountSettings.randomAnswerBox = m_randomAnswer->isChecked();
    accountSettings.callWaiting = m_callWaiting->isChecked();
    accountSettings.multiMonitor = m_multiMonitor->isChecked();
    accountSettings.networkChanges = m_networkChanges->isChecked();
    accountSettings.disableMessaging = m_disableMessaging->isChecked();
    accountSettings.disableNameLookup = m_disableNameLookup->isChecked();
    accountSettings.ringtone = m_ringtone->text();
    accountSettings.volumeRing = m_volumeRing->value();
    accountSettings.recordingPath = m_recordingPath->text().trimmed();
    accountSettings.recordingFormat = m_recMp3->isChecked() ? "mp3" : "wav";
    accountSettings.autoRecording = m_recAuto->isChecked();
    accountSettings.recordingButton = m_recButton->isChecked();
    accountSettings.enableLocalAccount = m_enableLocal->isChecked();

    int updIdx = m_updatesInterval->currentIndex();
    switch (updIdx) {
    case 0: accountSettings.updatesInterval = "daily"; break;
    case 2: accountSettings.updatesInterval = "monthly"; break;
    case 3: accountSettings.updatesInterval = "quarterly"; break;
    case 4: accountSettings.updatesInterval = "never"; break;
    default: accountSettings.updatesInterval = ""; break;
    }

    onyx_startup_set(m_startup->isChecked());
    accountSettings.SettingsSave();

    if (accountSettings.singleMode)
        mainDlg->messagesDlg->close();
#ifdef _GLOBAL_VIDEO
    mainDlg->messagesDlg->UpdateCallButton();
#endif
    mainDlg->messagesDlg->findChild<QWidget*>("IDC_MESSAGE")->setEnabled(!accountSettings.disableMessaging);
    mainDlg->pageDialer->RebuildButtons();
    mainDlg->pageDialer->UpdateCallButton();
    mainDlg->PJCreate();
    mainDlg->OnAccountChanged();
    mainDlg->PJAccountAdd();
    onClose();
}

// Codec movement helpers
void SettingsDlg::onDeltaposSpinModifyAdd() {
    int row = m_codecsAll->currentRow();
    if (row < 0) return;
    QListWidgetItem *item = m_codecsAll->takeItem(row);
    m_codecsEnabled->addItem(item);
    m_codecsEnabled->setCurrentItem(item);
}
void SettingsDlg::onDeltaposSpinModifyRemove() {
    int row = m_codecsEnabled->currentRow();
    if (row < 0) return;
    QListWidgetItem *item = m_codecsEnabled->takeItem(row);
    m_codecsAll->addItem(item);
    m_codecsAll->setCurrentItem(item);
}
void SettingsDlg::onDeltaposSpinOrderUp() {
    int row = m_codecsEnabled->currentRow();
    if (row <= 0) return;
    QListWidgetItem *item = m_codecsEnabled->takeItem(row);
    m_codecsEnabled->insertItem(row - 1, item);
    m_codecsEnabled->setCurrentRow(row - 1);
}
void SettingsDlg::onDeltaposSpinOrderDown() {
    int row = m_codecsEnabled->currentRow();
    if (row < 0 || row >= m_codecsEnabled->count() - 1) return;
    QListWidgetItem *item = m_codecsEnabled->takeItem(row);
    m_codecsEnabled->insertItem(row + 1, item);
    m_codecsEnabled->setCurrentRow(row + 1);
}

// Feature Codes
void SettingsDlg::onFeatureCodesClicked() {
    if (!featureCodesDlg) {
        featureCodesDlg = new FeatureCodesDlg(this);
        featureCodesDlg->show();
    } else {
        featureCodesDlg->raise();
        featureCodesDlg->activateWindow();
    }
}

// Ringtone
void SettingsDlg::onBnClickedBrowse() {
    QString file = QFileDialog::getOpenFileName(this, "", "", "WAV Files (*.wav)");
    if (file.isEmpty()) return;
    QDir curDir = QDir::current();
    QString rel = curDir.relativeFilePath(file);
    if (!rel.startsWith(".."))
        m_ringtone->setText(rel);
    else
        m_ringtone->setText(file);
}
void SettingsDlg::onChangeRingtone() {
    m_btnDefaultRing->setEnabled(!m_ringtone->text().isEmpty());
}
void SettingsDlg::onBnClickedDefault() {
    m_ringtone->clear();
}
void SettingsDlg::onVolumeRingChanged() {
    int oldVolume = accountSettings.volumeRing;
    accountSettings.volumeRing = m_volumeRing->value();
    QString ringtone = m_ringtone->text();
    if (ringtone.isEmpty()) ringtone = "ringtone.wav";
    mainDlg->onPlayerPlay(ONYX_SOUND_CUSTOM_NOLOOP, ringtone);
    accountSettings.volumeRing = oldVolume;
}

// Recording
void SettingsDlg::onBnClickedRecordingBrowse() {
    QString start = m_recordingPath->text();
    if (start.isEmpty() || QDir::isRelativePath(start))
        start = QDir::currentPath() + "/" + start;
    QString dir = QFileDialog::getExistingDirectory(this, "", start);
    if (!dir.isEmpty())
        m_recordingPath->setText(dir);
}
void SettingsDlg::onEnChangeRecording() {
    m_btnRecDefault->setEnabled(!m_recordingPath->text().isEmpty());
}
void SettingsDlg::onBnClickedRecordingDefault() {
    m_recordingPath->clear();
}

// AA Options
void SettingsDlg::onBnClickedAAOptions() {
    if (!aaOptionsDlg) {
        aaOptionsDlg = new AAOptionsDlg(this);
        aaOptionsDlg->show();
    } else {
        aaOptionsDlg->raise();
        aaOptionsDlg->activateWindow();
    }
}

// DNS & STUN checkboxes
void SettingsDlg::onBnClickedDnsSrv() {
    if (m_dnsSrvCheckbox->isChecked() && m_dnsSrvNs->text().isEmpty())
        m_dnsSrvNs->setText("8.8.8.8; 8.8.4.4");
}
void SettingsDlg::onBnClickedStun() {
    if (m_stunCheckbox->isChecked() && m_stun->text().isEmpty())
        m_stun->setText("stun.l.google.com:19302");
}

#ifdef _GLOBAL_VIDEO
void SettingsDlg::onBnClickedPreview() {
    QString name = m_vidCapDev->currentText();
    if (!mainDlg->previewWin) {
        mainDlg->previewWin = new Preview(mainDlg);
    }
    mainDlg->previewWin->Start(mainDlg->VideoCaptureDeviceId(name));
}
#endif

// Help handlers (all call OpenHelp(tag))
#define HELP_SLOT(tag) void SettingsDlg::onHelp##tag() { OpenHelp(#tag); }
HELP_SLOT(Ringtone)
HELP_SLOT(MicAmplif)
HELP_SLOT(SwAdjust)
HELP_SLOT(DtmfMethod)
HELP_SLOT(AutoAnswer)
HELP_SLOT(Forwarding)
HELP_SLOT(FeatureCodes)
HELP_SLOT(DenyIncoming)
HELP_SLOT(Directory)
HELP_SLOT(DnsSrv)
HELP_SLOT(StunServer)
HELP_SLOT(MediaButtons)
HELP_SLOT(HeadsetSupport)
HELP_SLOT(LocalDtmf)
HELP_SLOT(SingleMode)
HELP_SLOT(Vad)
HELP_SLOT(Ec)
HELP_SLOT(ForceCodec)
HELP_SLOT(Video)
HELP_SLOT(Ports)
HELP_SLOT(AudioCodecs)
HELP_SLOT(EnableLog)
HELP_SLOT(BringToFront)
HELP_SLOT(RandomAnswerBox)
HELP_SLOT(EnableLocal)
HELP_SLOT(CrashReport)

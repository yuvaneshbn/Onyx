#pragma once

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSlider>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "global.h"
#include "AAOptionsDlg.h"
#include "FeatureCodesDlg.h"

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget *parent = nullptr);
    ~SettingsDlg();

    AAOptionsDlg *aaOptionsDlg = nullptr;
    FeatureCodesDlg *featureCodesDlg = nullptr;

protected:
    QStringList autoAnswerValues;
    QStringList forwardingValues;
    QStringList denyIncomingValues;

    // UI controls – named after original resource IDs
    QLineEdit   *m_ringtone        = nullptr; // IDC_SETTINGS_RINGTONE
    QSlider     *m_volumeRing      = nullptr; // IDC_SETTINGS_VOLUME_RING
    QLineEdit   *m_recordingPath   = nullptr; // IDC_SETTINGS_RECORDING
    QRadioButton *m_recMp3         = nullptr; // IDC_SETTINGS_RECORDING_MP3
    QRadioButton *m_recWav         = nullptr; // IDC_SETTINGS_RECORDING_WAV
    QCheckBox   *m_recAuto         = nullptr; // IDC_SETTINGS_RECORDING_CHECKBOX
    QCheckBox   *m_recButton       = nullptr; // IDC_SETTINGS_RECORDING_BUTTON
    QComboBox   *m_audioInput      = nullptr; // IDC_SETTINGS_MICROPHONE
    QComboBox   *m_audioOutput     = nullptr; // IDC_SETTINGS_SPEAKERS
    QComboBox   *m_audioRing       = nullptr; // IDC_SETTINGS_RING
    QCheckBox   *m_micAmplif       = nullptr; // IDC_SETTINGS_MIC_AMPLIF
    QCheckBox   *m_swAdjust        = nullptr; // IDC_SETTINGS_SW_ADJUST
    QListWidget *m_codecsAll       = nullptr; // IDC_SETTINGS_AUDIO_CODECS_ALL
    QListWidget *m_codecsEnabled   = nullptr; // IDC_SETTINGS_AUDIO_CODECS
    QCheckBox   *m_vad             = nullptr; // IDC_SETTINGS_VAD
    QCheckBox   *m_ec              = nullptr; // IDC_SETTINGS_EC
    QCheckBox   *m_opusStereo      = nullptr; // IDC_SETTINGS_OPUS_STEREO
    QCheckBox   *m_forceCodec      = nullptr; // IDC_SETTINGS_FORCE_CODEC
#ifdef _GLOBAL_VIDEO
    QCheckBox   *m_disableVideo    = nullptr; // IDC_SETTINGS_DISABLE_VIDEO
    QComboBox   *m_vidCapDev       = nullptr; // IDC_SETTINGS_VID_CAP_DEV
    QComboBox   *m_videoCodec      = nullptr; // IDC_SETTINGS_VIDEO_CODEC
    QCheckBox   *m_videoH264       = nullptr; // IDC_SETTINGS_VIDEO_H264
    QCheckBox   *m_videoH263       = nullptr; // IDC_SETTINGS_VIDEO_H263
    QCheckBox   *m_videoVP8        = nullptr; // IDC_SETTINGS_VIDEO_VP8
    QCheckBox   *m_videoVP9        = nullptr; // IDC_SETTINGS_VIDEO_VP9
    QLineEdit   *m_videoBitrate    = nullptr; // IDC_SETTINGS_VIDEO_BITRATE
    QPushButton *m_previewBtn      = nullptr; // IDC_SETTINGS_PREVIEW
#endif
    QCheckBox   *m_rport           = nullptr; // IDC_SETTINGS_RPORT
    QLineEdit   *m_sourcePort      = nullptr; // IDC_SETTINGS_SOURCE_PORT
    QLineEdit   *m_rtpPortMin      = nullptr; // IDC_SETTINGS_RTP_PORT_MIN
    QLineEdit   *m_rtpPortMax      = nullptr; // IDC_SETTINGS_RTP_PORT_MAX
    QLineEdit   *m_dnsSrvNs        = nullptr; // IDC_SETTINGS_DNS_SRV_NS
    QCheckBox   *m_dnsSrvCheckbox  = nullptr; // IDC_SETTINGS_DNS_SRV_CHECKBOX
    QLineEdit   *m_stun            = nullptr; // IDC_SETTINGS_STUN
    QCheckBox   *m_stunCheckbox    = nullptr; // IDC_SETTINGS_STUN_CHECKBOX
    QComboBox   *m_dtmfMethod      = nullptr; // IDC_SETTINGS_DTMF_METHOD
    QComboBox   *m_autoAnswer      = nullptr; // IDC_SETTINGS_AUTO_ANSWER
    QComboBox   *m_forwarding      = nullptr; // IDC_SETTINGS_FWD
    QLineEdit   *m_fwdNumber       = nullptr; // IDC_SETTINGS_FWD_NUMBER
    QLineEdit   *m_fwdDelay        = nullptr; // IDC_SETTINGS_FWD_DELAY
    QComboBox   *m_denyIncoming    = nullptr; // IDC_SETTINGS_DENY_INCOMING
    QLineEdit   *m_directory       = nullptr; // IDC_SETTINGS_DIRECTORY
    QComboBox   *m_defaultAction   = nullptr; // IDC_SETTINGS_DEFAULT_ACTION
    QCheckBox   *m_mediaButtons    = nullptr; // IDC_SETTINGS_MEDIA_BUTTONS
    QCheckBox   *m_headset         = nullptr; // IDC_SETTINGS_HID
    QCheckBox   *m_localDtmf       = nullptr; // IDC_SETTINGS_LOCAL_DTMF
    QCheckBox   *m_singleMode      = nullptr; // IDC_SETTINGS_SINGLE_MODE
    QCheckBox   *m_enableLog       = nullptr; // IDC_SETTINGS_ENABLE_LOG
    QCheckBox   *m_bringToFront    = nullptr; // IDC_SETTINGS_BRING_TO_FRONT
    QCheckBox   *m_randomAnswer    = nullptr; // IDC_SETTINGS_ANSWER_BOX_RANDOM
    QCheckBox   *m_callWaiting     = nullptr; // IDC_SETTINGS_CALL_WAITING
    QCheckBox   *m_multiMonitor    = nullptr; // IDC_SETTINGS_MULTI_MONITOR
    QCheckBox   *m_networkChanges  = nullptr; // IDC_SETTINGS_NETWORK_CHANGES
    QCheckBox   *m_disableMessaging= nullptr; // IDC_SETTINGS_DISMESS
    QCheckBox   *m_disableNameLookup= nullptr; // IDC_SETTINGS_DISABLE_NAME_LOOKUP
    QCheckBox   *m_enableLocal     = nullptr; // IDC_SETTINGS_ENABLE_LOCAL
    QComboBox   *m_updatesInterval = nullptr; // IDC_SETTINGS_UPDATES_INTERVAL
    QCheckBox   *m_startup         = nullptr; // IDC_SETTINGS_STARTUP
    QPushButton *m_btnFeatureCodes = nullptr; // IDC_SETTINGS_FEATURE_CODES
    QPushButton *m_btnBrowseRing   = nullptr; // IDC_SETTINGS_BROWSE
    QPushButton *m_btnDefaultRing  = nullptr; // IDC_SETTINGS_DEFAULT
    QPushButton *m_btnRecBrowse    = nullptr; // IDC_SETTINGS_RECORDING_BROWSE
    QPushButton *m_btnRecDefault   = nullptr; // IDC_SETTINGS_RECORDING_DEFAULT
    QPushButton *m_btnAAOptions    = nullptr; // IDC_SETTINGS_AA_OPTIONS

    void setupUi();
    void translateUi();
    void initDialog();
    void populateAudioDevices();
    void populateCodecLists();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onClose();
    void onBnClickedCancel();
    void onBnClickedOk();
    void onUpdateSettings();
    void onDeltaposSpinModifyAdd();
    void onDeltaposSpinModifyRemove();
    void onDeltaposSpinOrderUp();
    void onDeltaposSpinOrderDown();
    void onFeatureCodesClicked();
    // Help handlers
    void onHelpRingtone();
    void onHelpMicAmplif();
    void onHelpSwAdjust();
    void onHelpDtmfMethod();
    void onHelpAutoAnswer();
    void onHelpForwarding();
    void onHelpFeatureCodes();
    void onHelpDenyIncoming();
    void onHelpDirectory();
    void onHelpDnsSrv();
    void onHelpStunServer();
    void onHelpMediaButtons();
    void onHelpHeadsetSupport();
    void onHelpLocalDtmf();
    void onHelpSingleMode();
    void onHelpVad();
    void onHelpEc();
    void onHelpForceCodec();
    void onHelpVideo();
    void onHelpPorts();
    void onHelpAudioCodecs();
    void onHelpEnableLog();
    void onHelpBringToFront();
    void onHelpRandomAnswerBox();
    void onHelpEnableLocal();
    void onHelpCrashReport();
    void onBnClickedBrowse();
    void onChangeRingtone();
    void onBnClickedDefault();
    void onVolumeRingChanged();
    void onBnClickedRecordingBrowse();
    void onEnChangeRecording();
    void onBnClickedRecordingDefault();
    void onBnClickedAAOptions();
    void onBnClickedDnsSrv();
    void onBnClickedStun();
#ifdef _GLOBAL_VIDEO
    void onBnClickedPreview();
#endif
};

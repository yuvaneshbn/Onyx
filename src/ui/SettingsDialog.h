#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

namespace Onyx {

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

    // Audio getters
    int inputVolume() const;
    float inputGainDb() const;
    int outputVolume() const;
    QString audioCodec() const;
    bool noiseSuppression() const;
    float vadThreshold() const;

    // Video getters
    QSize videoResolution() const;
    int frameRate() const;
    QString videoCodec() const;
    int videoBitrateKbps() const;

    // Notification getters
    bool desktopNotifications() const;
    bool callRingtone() const;
    bool messageAlert() const;
    int notificationVolume() const;

    // Security getters
    QString roomPasskey() const;
    QByteArray derivedKey() const;
    QString securityFingerprint() const;

    void updateMicTestLevel(float level);

signals:
    void settingsApplied();

private slots:
    void onPasskeyChanged(const QString& passkey);
    void onSave();

private:
    void setupUi();
    QWidget* createAudioTab();
    QWidget* createVideoTab();
    QWidget* createNotificationsTab();
    QWidget* createSecurityTab();

    void loadSettings();
    void saveSettings();

    // Audio widgets
    QComboBox* m_micDeviceCombo = nullptr;
    QSlider* m_inputVolSlider = nullptr;
    QLabel* m_inputVolLabel = nullptr;
    QSlider* m_inputGainSlider = nullptr;
    QLabel* m_inputGainLabel = nullptr;
    QProgressBar* m_micLevelBar = nullptr;
    QComboBox* m_speakerDeviceCombo = nullptr;
    QSlider* m_outputVolSlider = nullptr;
    QLabel* m_outputVolLabel = nullptr;
    QComboBox* m_audioCodecCombo = nullptr;
    QCheckBox* m_noiseSuppressionCheck = nullptr;
    QSlider* m_vadSlider = nullptr;
    QLabel* m_vadLabel = nullptr;

    // Video widgets
    QComboBox* m_cameraDeviceCombo = nullptr;
    QComboBox* m_resolutionCombo = nullptr;
    QComboBox* m_frameRateCombo = nullptr;
    QComboBox* m_videoCodecCombo = nullptr;
    QSlider* m_bitrateSlider = nullptr;
    QLabel* m_bitrateLabel = nullptr;

    // Notification widgets
    QCheckBox* m_desktopNotifyCheck = nullptr;
    QCheckBox* m_ringtoneCheck = nullptr;
    QCheckBox* m_msgAlertCheck = nullptr;
    QSlider* m_notifyVolSlider = nullptr;

    // Security widgets
    QLineEdit* m_passkeyEdit = nullptr;
    QLabel* m_fingerprintLabel = nullptr;
    QLabel* m_keyPreviewLabel = nullptr;
    QLabel* m_securityBadge = nullptr;
};

} // namespace Onyx

#endif // SETTINGS_DIALOG_H

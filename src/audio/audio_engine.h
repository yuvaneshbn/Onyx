#ifndef AUDIO_AUDIO_ENGINE_H
#define AUDIO_AUDIO_ENGINE_H

#include <string>
#include <vector>
#include <memory>

#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include "engine.h"
#include <QUdpSocket>

class QAudioSource;
class QAudioSink;
class QIODevice;
class QAudioFormat;
class QAudioDevice;

// Lightweight stub interface used by the Qt UI layer.
// Bridge to the real audio::Engine and Qt Multimedia I/O.
struct DeviceInfo {
    std::string name;
    int index;
};

class AudioEngine : public QObject {
    Q_OBJECT
public:
    AudioEngine(QObject* parent = nullptr);
    ~AudioEngine() override;

    void setClientId(const std::string& id);
    void resetEchoCanceller(int /*delay_ms*/);

    bool isRunning() const;
    void start(const std::string& ip);
    void stop();
    void shutdown();

    void updateDestinations(const std::vector<std::string>& hosts);
    int port() const;

    void setTxMuted(bool muted);
    void setAllowTx(bool allowed);
    bool isTxMuted() const;
    bool captureActive() const;
    int captureLevel() const;
    float mixedPeak() const;

    // Audio controls
    void setMasterVolume(int value);
    void setGainDb(int value);
    void setNoiseSuppression(int value);
    void setNoiseSuppressionEnabled(bool enabled);
    void setEchoEnabled(bool enabled);
    bool echoEnabled() const;
    bool echoAvailable() const;
    int testMicrophoneLevel(double scale);

    // Device selection
    std::vector<DeviceInfo> listInputDevices() const;
    std::vector<DeviceInfo> listOutputDevices() const;
    int inputDeviceIndex() const;
    int outputDeviceIndex() const;
    void setInputDevice(int index);
    void setOutputDevice(int index);

private:
    void openDevices();
    void closeDevices();
    void onCaptureReady();
    void onNetworkReady();
    void onPlaybackTimer();
    QAudioDevice inputDeviceByIndex(int idx) const;
    QAudioDevice outputDeviceByIndex(int idx) const;

    std::unique_ptr<QAudioFormat> format_;
    std::unique_ptr<QAudioSource> audio_input_;
    std::unique_ptr<QAudioSink> audio_output_;
    QIODevice* mic_io_{nullptr};
    QIODevice* spk_io_{nullptr};
    std::unique_ptr<QUdpSocket> udp_;

    std::vector<char> capture_buffer_;
    std::vector<float> float_buffer_;
    std::vector<float> capture_accum_;
    std::vector<std::pair<QHostAddress, quint16>> dest_addrs_;

    audio::Engine engine_;

    std::string client_id_;
    bool running_{false};
    bool tx_muted_{false};
    bool allow_tx_{true};
    bool capture_active_{false};
    int capture_level_{0};
    float mixed_peak_{0.0f};
    int master_volume_{100};
    int gain_db_{0};
    int noise_suppression_{0};
    bool ns_enabled_{false};
    bool echo_enabled_{false};
    uint16_t tx_seq_{0};
    quint16 port_{0};
    uint32_t local_ssrc_{0};
    int input_index_{0};
    int output_index_{0};
    std::vector<std::string> destinations_;
    QTimer playback_timer_;
};

#endif

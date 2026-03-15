#include "audio_engine.h"

#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QAudioSource>
#include <QMediaDevices>
#include <QHostAddress>
#include <QTimer>
#include <QNetworkInterface>
#include <QtEndian>
#include <QtNetwork/QUdpSocket>
#include <QIODevice>

#include <algorithm>
#include <cstring>
#include <algorithm>
#include <cstring>

static constexpr quint16 kAudioPort = 47000;

AudioEngine::AudioEngine(QObject* parent)
    : QObject(parent), format_(std::make_unique<QAudioFormat>()), engine_(this) {
    format_->setSampleRate(48000);
    format_->setChannelCount(1);
    format_->setSampleFormat(QAudioFormat::Float);
    capture_buffer_.resize(48000); // 1s max buffer

    // Keep UDP open permanently so we can always receive and port is known
    udp_ = std::make_unique<QUdpSocket>(this);
    if (udp_->bind(QHostAddress::AnyIPv4, 0)) {
        port_ = udp_->localPort();
        connect(udp_.get(), &QUdpSocket::readyRead, this, &AudioEngine::onNetworkReady);
    }

    // Forward encoded packets to UDP
    connect(&engine_, &audio::Engine::localPacketReady, this, [this](const std::vector<uint8_t>& payload) {
        if (!udp_) return;
        if (tx_muted_ || !allow_tx_) return;
        tx_seq_++;

        // Packet format: [4 bytes SSRC][2 bytes seq][payload]
        QByteArray datagram;
        datagram.resize(static_cast<int>(payload.size() + sizeof(uint32_t) + sizeof(uint16_t)));
        qToBigEndian<uint32_t>(local_ssrc_, reinterpret_cast<uchar*>(datagram.data()));
        qToBigEndian<uint16_t>(tx_seq_, reinterpret_cast<uchar*>(datagram.data() + sizeof(uint32_t)));
        std::memcpy(datagram.data() + sizeof(uint32_t) + sizeof(uint16_t), payload.data(), payload.size());

        if (dest_addrs_.empty()) {
            const quint16 dstPort = kAudioPort;
            udp_->writeDatagram(datagram, QHostAddress::Broadcast, dstPort);
            udp_->writeDatagram(datagram, QHostAddress::LocalHost, dstPort);
        } else {
            for (const auto& endpoint : dest_addrs_) {
                udp_->writeDatagram(datagram, endpoint.first, endpoint.second);
            }
        }
    });

    // Poll playback path even if capture is silent/not active.
    playback_timer_.setInterval(20); // 20ms frames
    playback_timer_.callOnTimeout([this]() { onPlaybackTimer(); });
}

AudioEngine::~AudioEngine() {
    shutdown();
}

void AudioEngine::setClientId(const std::string& id) {
    client_id_ = id;
    local_ssrc_ = static_cast<uint32_t>(qHash(QString::fromStdString(id)));
}
void AudioEngine::resetEchoCanceller(int) {}

bool AudioEngine::isRunning() const { return running_; }

void AudioEngine::start(const std::string&) {
    if (running_) return;
    openDevices();
    engine_.start();
    running_ = true;
    capture_active_ = true;
    playback_timer_.start();
}

void AudioEngine::stop() {
    if (!running_) return;
    engine_.stop();
    closeDevices();
    running_ = false;
    capture_active_ = false;
    playback_timer_.stop();
}

void AudioEngine::shutdown() { stop(); }

void AudioEngine::updateDestinations(const std::vector<std::string>& hosts) {
    dest_addrs_.clear();
    for (const auto& h : hosts) {
        QString qh = QString::fromStdString(h);
        QString host = qh;
        quint16 port = kAudioPort;
        auto parts = qh.split(':');
        if (parts.size() == 2) {
            host = parts[0];
            port = static_cast<quint16>(parts[1].toUShort());
        }
        dest_addrs_.push_back({QHostAddress(host), port});
    }
    destinations_ = hosts;
}

int AudioEngine::port() const { return port_; }

void AudioEngine::setTxMuted(bool muted) { tx_muted_ = muted; }
void AudioEngine::setAllowTx(bool allowed) { allow_tx_ = allowed; }
bool AudioEngine::isTxMuted() const { return tx_muted_; }

bool AudioEngine::captureActive() const { return capture_active_; }
int AudioEngine::captureLevel() const { return capture_level_; }
float AudioEngine::mixedPeak() const { return mixed_peak_; }

void AudioEngine::setMasterVolume(int value) { master_volume_ = std::clamp(value, 0, 100); }
void AudioEngine::setGainDb(int value) { gain_db_ = value; }
void AudioEngine::setNoiseSuppression(int value) { noise_suppression_ = value; }
void AudioEngine::setNoiseSuppressionEnabled(bool enabled) { ns_enabled_ = enabled; }
void AudioEngine::setEchoEnabled(bool enabled) { echo_enabled_ = enabled; }
bool AudioEngine::echoEnabled() const { return echo_enabled_; }
bool AudioEngine::echoAvailable() const { return true; }

int AudioEngine::testMicrophoneLevel(double scale) {
    capture_level_ = static_cast<int>(std::clamp(scale * capture_level_, 0.0, 100.0));
    return capture_level_;
}

std::vector<DeviceInfo> AudioEngine::listInputDevices() const {
    std::vector<DeviceInfo> out;
    const auto devices = QMediaDevices::audioInputs();
    int idx = 0;
    for (const auto& dev : devices) {
        out.push_back({dev.description().toStdString(), idx++});
    }
    return out;
}

std::vector<DeviceInfo> AudioEngine::listOutputDevices() const {
    std::vector<DeviceInfo> out;
    const auto devices = QMediaDevices::audioOutputs();
    int idx = 0;
    for (const auto& dev : devices) {
        out.push_back({dev.description().toStdString(), idx++});
    }
    return out;
}

int AudioEngine::inputDeviceIndex() const { return input_index_; }
int AudioEngine::outputDeviceIndex() const { return output_index_; }

void AudioEngine::setInputDevice(int index) {
    input_index_ = index;
    if (running_) {
        closeDevices();
        openDevices();
    }
}

void AudioEngine::setOutputDevice(int index) {
    output_index_ = index;
    if (running_) {
        closeDevices();
        openDevices();
    }
}

QAudioDevice AudioEngine::inputDeviceByIndex(int idx) const {
    const auto devices = QMediaDevices::audioInputs();
    if (idx >= 0 && idx < devices.size()) return devices.at(idx);
    return QMediaDevices::defaultAudioInput();
}

QAudioDevice AudioEngine::outputDeviceByIndex(int idx) const {
    const auto devices = QMediaDevices::audioOutputs();
    if (idx >= 0 && idx < devices.size()) return devices.at(idx);
    return QMediaDevices::defaultAudioOutput();
}

void AudioEngine::openDevices() {
    closeDevices();

    auto inDev = inputDeviceByIndex(input_index_);
    auto outDev = outputDeviceByIndex(output_index_);

    audio_input_ = std::make_unique<QAudioSource>(inDev, *format_, this);
    audio_output_ = std::make_unique<QAudioSink>(outDev, *format_, this);

    mic_io_ = audio_input_->start();
    spk_io_ = audio_output_->start();

    // Connect capture readyRead
    connect(mic_io_, &QIODevice::readyRead, this, &AudioEngine::onCaptureReady);
}

void AudioEngine::closeDevices() {
    if (mic_io_) {
        disconnect(mic_io_, nullptr, this, nullptr);
    }
    mic_io_ = nullptr;
    spk_io_ = nullptr;
    udp_.reset();
    audio_input_.reset();
    audio_output_.reset();
}

void AudioEngine::onCaptureReady() {
    if (!mic_io_ || !spk_io_) return;
    const qint64 bytes = std::min<qint64>(mic_io_->bytesAvailable(), static_cast<qint64>(capture_buffer_.size()));
    if (bytes <= 0) return;

    const qint64 read = mic_io_->read(capture_buffer_.data(), bytes);
    if (read <= 0) return;

    const int frames = static_cast<int>(read / sizeof(float));
    float_buffer_.resize(frames);
    std::memcpy(float_buffer_.data(), capture_buffer_.data(), read);

    // Update level
    float peak = 0.f;
    for (int i = 0; i < frames; ++i) {
        peak = std::max(peak, std::abs(float_buffer_[i]));
    }
    mixed_peak_ = peak;
    capture_level_ = static_cast<int>(std::clamp(peak * 100.f, 0.f, 100.f));

    // Accumulate and encode in 20ms (960-sample) frames
    constexpr int kFrame = 960;
    capture_accum_.insert(capture_accum_.end(), float_buffer_.begin(), float_buffer_.end());

    while (static_cast<int>(capture_accum_.size()) >= kFrame) {
        if (!tx_muted_ && allow_tx_) {
            engine_.processCaptureBuffer(capture_accum_.data(), kFrame);
        }

        capture_accum_.erase(capture_accum_.begin(), capture_accum_.begin() + kFrame);
    }
}

void AudioEngine::onNetworkReady() {
    if (!udp_) return;
    while (udp_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(udp_->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort = 0;
        udp_->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (datagram.size() < static_cast<int>(sizeof(uint32_t) + sizeof(uint16_t))) continue;

        uint32_t remote_ssrc = qFromBigEndian<uint32_t>(reinterpret_cast<const uchar*>(datagram.constData()));
        if (remote_ssrc == local_ssrc_) {
            continue; // Ignore our own packets
        }

        uint16_t seq = qFromBigEndian<uint16_t>(reinterpret_cast<const uchar*>(datagram.constData() + sizeof(uint32_t)));
        std::vector<uint8_t> payload(datagram.begin() + sizeof(uint32_t) + sizeof(uint16_t), datagram.end());

        // Accept packets from any source; self-filtering is handled via SSRC.
        engine_.receiveRemotePacket(remote_ssrc, seq, payload);
    }
}

void AudioEngine::onPlaybackTimer() {
    if (!running_ || !spk_io_) {
        return;
    }
    constexpr int kFrame = 960;
    std::vector<float> playback_frame(kFrame, 0.0f);
    engine_.processPlaybackBuffer(playback_frame.data(), kFrame);

    // If non-silent, write to speaker output device
    if (spk_io_ && !playback_frame.empty()) {
        const qint64 bytesToWrite = kFrame * sizeof(float);
        spk_io_->write(reinterpret_cast<const char*>(playback_frame.data()), bytesToWrite);
    }
}

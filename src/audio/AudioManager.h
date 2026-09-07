#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include "OpusAudioPipeline.h"
#include "AmrCodecManager.h"
#include "../filetransfer/CryptoEngine.h"
#include "../core/NetworkConfig.h"

namespace Onyx {

class AudioManager : public QObject {
    Q_OBJECT
public:
    enum CodecType {
        CodecOpus,
        CodecAmrWb,
        CodecAmrNb
    };

    explicit AudioManager(QObject* parent = nullptr);
    ~AudioManager() override;

    bool startCapture();
    void stopCapture();

    bool startPlayback();
    void stopPlayback();

    void setMuted(bool muted);
    bool isMuted() const { return m_muted; }

    void setCodec(CodecType codec);
    CodecType codec() const { return m_codec; }

    // Volume & Gain controls
    void setInputVolume(int percent); // 0 - 100
    int inputVolume() const { return m_inputVolumePercent; }

    void setInputGainDb(float gainDb); // 0.0 to 20.0 dB
    float inputGainDb() const { return m_inputGainDb; }

    void setOutputVolume(int percent); // 0 - 100
    int outputVolume() const { return m_outputVolumePercent; }

    void setVadThreshold(float thresh) { m_vadThreshold = thresh; }
    void setNoiseSuppressionEnabled(bool enabled) { m_noiseSuppression = enabled; }

    void setEncryptionKey(const QByteArray& key) { m_encryptionKey = key; }
    void setLocalPeerId(const QString& peerId) { m_localPeerId = peerId; }

    void setTargetPeer(const QString& host, quint16 rtpPort);
    void clearTargetPeer();
    void setGroupCallMode(bool enabled);

    bool bindLocalMediaPort(quint16 port = GROUP_AUDIO_PORT);

    static QStringList enumerateInputDevices();
    static QStringList enumerateOutputDevices();

signals:
    void micLevelChanged(float level); // 0.0 to 1.0
    void speakingStateChanged(bool isSpeaking);
    void peerSpeakingStateChanged(const QString& peerId, bool isSpeaking);
    void audioPacketReady(const QByteArray& encodedPacket);
    void audioError(const QString& errorMsg);

private slots:
    void onIncomingUdpDatagram();
    void onCapturePoll();

private:
    static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    void handleWaveInData(WAVEHDR* pHdr);

    void processCapturedSamples(const qint16* samples, int count);
    void playDecodedSamples(const qint16* samples, int count);

    HWAVEIN m_hWaveIn = nullptr;
    HWAVEOUT m_hWaveOut = nullptr;

    static constexpr int NUM_IN_BUFFERS = 4;
    static constexpr int IN_BUFFER_BYTES = OPUS_SAMPLES_PER_FRAME * sizeof(qint16); // 1920 bytes (20ms)
    WAVEHDR m_inWaveHdr[NUM_IN_BUFFERS];
    char m_inBuffers[NUM_IN_BUFFERS][IN_BUFFER_BYTES];

    QUdpSocket* m_udpSocket = nullptr;
    QHostAddress m_targetAddress;
    quint16 m_targetPort = 0;

    OpusAudioPipeline m_opusPipeline;
    AmrCodecManager m_amrManager;
    CodecType m_codec = CodecOpus;

    // Per-peer decoders for group call mixing
    struct PeerDecoderState {
        OpusAudioPipeline* opus = nullptr;
        qint64 lastAudioTime = 0;
        bool isSpeaking = false;
    };
    QMap<QString, PeerDecoderState> m_peerDecoders;

    QByteArray m_encryptionKey;
    QString m_localPeerId = "local_peer";
    bool m_groupMode = true;

    int m_inputVolumePercent = 100;
    float m_inputGainDb = 0.0f;
    float m_inputGainMultiplier = 1.0f;
    int m_outputVolumePercent = 100;
    float m_outputVolumeMultiplier = 1.0f;
    float m_vadThreshold = 0.02f;
    bool m_noiseSuppression = true;
    bool m_isSpeaking = false;

    bool m_capturing = false;
    bool m_playing = false;
    bool m_muted = false;
};

} // namespace Onyx

#endif // AUDIO_MANAGER_H

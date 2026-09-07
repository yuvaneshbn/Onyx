#include "AudioManager.h"
#include <cmath>
#include <QDateTime>
#include <QDebug>

#pragma comment(lib, "winmm.lib")

namespace Onyx {

AudioManager::AudioManager(QObject* parent)
    : QObject(parent),
      m_opusPipeline(AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, 64000) {
    m_opusPipeline.initialize();
    m_amrManager.initialize();

    memset(m_inWaveHdr, 0, sizeof(m_inWaveHdr));
}

AudioManager::~AudioManager() {
    stopCapture();
    stopPlayback();
    for (auto& state : m_peerDecoders) {
        if (state.opus) {
            delete state.opus;
            state.opus = nullptr;
        }
    }
    m_peerDecoders.clear();

    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }
}

void AudioManager::setInputVolume(int percent) {
    m_inputVolumePercent = qBound(0, percent, 100);
    m_inputGainMultiplier = std::pow(10.0f, m_inputGainDb / 20.0f) * (m_inputVolumePercent / 100.0f);
}

void AudioManager::setInputGainDb(float gainDb) {
    m_inputGainDb = qBound(0.0f, gainDb, 20.0f);
    m_inputGainMultiplier = std::pow(10.0f, m_inputGainDb / 20.0f) * (m_inputVolumePercent / 100.0f);
}

void AudioManager::setOutputVolume(int percent) {
    m_outputVolumePercent = qBound(0, percent, 100);
    m_outputVolumeMultiplier = m_outputVolumePercent / 100.0f;
}

void AudioManager::setGroupCallMode(bool enabled) {
    m_groupMode = enabled;
}

QStringList AudioManager::enumerateInputDevices() {
    QStringList list;
    UINT numDevs = waveInGetNumDevs();
    for (UINT i = 0; i < numDevs; ++i) {
        WAVEINCAPSW caps;
        if (waveInGetDevCapsW(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
            list.append(QString::fromWCharArray(caps.szPname));
        }
    }
    if (list.isEmpty()) {
        list.append("Default Microphone");
    }
    return list;
}

QStringList AudioManager::enumerateOutputDevices() {
    QStringList list;
    UINT numDevs = waveOutGetNumDevs();
    for (UINT i = 0; i < numDevs; ++i) {
        WAVEOUTCAPSW caps;
        if (waveOutGetDevCapsW(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
            list.append(QString::fromWCharArray(caps.szPname));
        }
    }
    if (list.isEmpty()) {
        list.append("Default Audio Output");
    }
    return list;
}

bool AudioManager::bindLocalMediaPort(quint16 port) {
    if (!m_udpSocket) {
        m_udpSocket = new QUdpSocket(this);
        connect(m_udpSocket, &QUdpSocket::readyRead, this, &AudioManager::onIncomingUdpDatagram);
    }

    if (m_udpSocket->state() == QAbstractSocket::BoundState) {
        m_udpSocket->close();
    }

    bool bound = m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (bound) {
        m_udpSocket->joinMulticastGroup(QHostAddress(MULTICAST_GROUP));
    }
    return bound;
}

void AudioManager::setTargetPeer(const QString& host, quint16 rtpPort) {
    m_targetAddress = QHostAddress(host);
    m_targetPort = rtpPort;
}

void AudioManager::clearTargetPeer() {
    m_targetAddress.clear();
    m_targetPort = 0;
}

void CALLBACK AudioManager::waveInProc(HWAVEIN, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR) {
    if (uMsg == WIM_DATA) {
        AudioManager* mgr = reinterpret_cast<AudioManager*>(dwInstance);
        WAVEHDR* pHdr = reinterpret_cast<WAVEHDR*>(dwParam1);
        if (mgr && pHdr) {
            mgr->handleWaveInData(pHdr);
        }
    }
}

void AudioManager::handleWaveInData(WAVEHDR* pHdr) {
    if (!m_capturing || !m_hWaveIn) return;

    if (pHdr->dwBytesRecorded > 0) {
        const qint16* samples = reinterpret_cast<const qint16*>(pHdr->lpData);
        int sampleCount = pHdr->dwBytesRecorded / sizeof(qint16);
        processCapturedSamples(samples, sampleCount);
    }

    // Re-queue buffer
    waveInAddBuffer(m_hWaveIn, pHdr, sizeof(WAVEHDR));
}

bool AudioManager::startCapture() {
    stopCapture();

    WAVEFORMATEX wfx;
    memset(&wfx, 0, sizeof(wfx));
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = AUDIO_CHANNELS;
    wfx.nSamplesPerSec = AUDIO_SAMPLE_RATE;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    MMRESULT res = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &wfx,
                              reinterpret_cast<DWORD_PTR>(waveInProc),
                              reinterpret_cast<DWORD_PTR>(this),
                              CALLBACK_FUNCTION);
    if (res != MMSYSERR_NOERROR) {
        emit audioError("Failed to open Windows audio input device");
        return false;
    }

    for (int i = 0; i < NUM_IN_BUFFERS; ++i) {
        memset(&m_inWaveHdr[i], 0, sizeof(WAVEHDR));
        m_inWaveHdr[i].lpData = m_inBuffers[i];
        m_inWaveHdr[i].dwBufferLength = IN_BUFFER_BYTES;
        waveInPrepareHeader(m_hWaveIn, &m_inWaveHdr[i], sizeof(WAVEHDR));
        waveInAddBuffer(m_hWaveIn, &m_inWaveHdr[i], sizeof(WAVEHDR));
    }

    m_capturing = true;
    waveInStart(m_hWaveIn);
    return true;
}

void AudioManager::stopCapture() {
    if (m_hWaveIn) {
        m_capturing = false;
        waveInReset(m_hWaveIn);
        for (int i = 0; i < NUM_IN_BUFFERS; ++i) {
            waveInUnprepareHeader(m_hWaveIn, &m_inWaveHdr[i], sizeof(WAVEHDR));
        }
        waveInClose(m_hWaveIn);
        m_hWaveIn = nullptr;
    }
}

bool AudioManager::startPlayback() {
    stopPlayback();

    WAVEFORMATEX wfx;
    memset(&wfx, 0, sizeof(wfx));
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = AUDIO_CHANNELS;
    wfx.nSamplesPerSec = AUDIO_SAMPLE_RATE;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    MMRESULT res = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if (res != MMSYSERR_NOERROR) {
        emit audioError("Failed to open Windows audio output device");
        return false;
    }

    m_playing = true;
    return true;
}

void AudioManager::stopPlayback() {
    if (m_hWaveOut) {
        m_playing = false;
        waveOutReset(m_hWaveOut);
        waveOutClose(m_hWaveOut);
        m_hWaveOut = nullptr;
    }
}

void AudioManager::setMuted(bool muted) {
    m_muted = muted;
}

void AudioManager::setCodec(CodecType codec) {
    m_codec = codec;
}

void AudioManager::onCapturePoll() {
}

void AudioManager::processCapturedSamples(const qint16* rawSamples, int count) {
    // Apply Input Gain & Volume Scaling
    QVector<qint16> scaledSamples(count);
    float sumSquares = 0.0f;
    for (int i = 0; i < count; ++i) {
        float sampleF = static_cast<float>(rawSamples[i]) * m_inputGainMultiplier;
        // Clipping protection
        if (sampleF > 32767.0f) sampleF = 32767.0f;
        else if (sampleF < -32768.0f) sampleF = -32768.0f;
        scaledSamples[i] = static_cast<qint16>(sampleF);

        float norm = sampleF / 32768.0f;
        sumSquares += norm * norm;
    }

    float rms = std::sqrt(sumSquares / count);
    emit micLevelChanged(std::min(1.0f, rms * 4.0f));

    // VAD (Voice Activity Detection)
    bool nowSpeaking = (!m_muted && rms > m_vadThreshold);
    if (nowSpeaking != m_isSpeaking) {
        m_isSpeaking = nowSpeaking;
        emit speakingStateChanged(m_isSpeaking);
    }

    if (m_muted) return;

    // Encode audio frame
    QByteArray rawPayload;
    char codecTag = 'O';
    if (m_codec == CodecOpus) {
        codecTag = 'O';
        m_opusPipeline.encodeFrame(scaledSamples.constData(), count, rawPayload);
    } else {
        codecTag = 'A';
        QVector<qint16> pcm16k = AmrCodecManager::resample48To16(scaledSamples.constData(), count);
        m_amrManager.encodeFrame(pcm16k.constData(), pcm16k.size(), rawPayload);
    }

    if (rawPayload.isEmpty()) return;

    // Encrypt audio payload if E2EE encryption key is set
    QByteArray finalPayload = rawPayload;
    if (!m_encryptionKey.isEmpty()) {
        QByteArray encrypted = CryptoEngine::encryptEnvelope(rawPayload, m_encryptionKey);
        if (!encrypted.isEmpty()) {
            finalPayload = encrypted;
        }
    }

    // Pack into packet: [4 bytes Magic "ONYX"] + [1 byte Codec] + [1 byte PeerIdLen] + [PeerId] + [Payload]
    QByteArray packet;
    packet.append("ONYX", 4);
    packet.append(codecTag);
    QByteArray idBytes = m_localPeerId.toUtf8();
    packet.append(static_cast<char>(idBytes.size()));
    packet.append(idBytes);
    packet.append(finalPayload);

    emit audioPacketReady(packet);

    if (m_udpSocket) {
        if (m_groupMode) {
            // Multicast to all group participants
            m_udpSocket->writeDatagram(packet, QHostAddress(MULTICAST_GROUP), GROUP_AUDIO_PORT);
        } else if (!m_targetAddress.isNull() && m_targetPort > 0) {
            // Unicast to target peer
            m_udpSocket->writeDatagram(packet, m_targetAddress, m_targetPort);
        }
    }
}

void AudioManager::onIncomingUdpDatagram() {
    while (m_udpSocket && m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 port;
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &port);

        if (datagram.size() < 6) continue;

        // Check Onyx header
        if (datagram.left(4) != "ONYX") continue;

        char codecTag = datagram.at(4);
        int idLen = static_cast<unsigned char>(datagram.at(5));
        if (datagram.size() < 6 + idLen) continue;

        QString peerId = QString::fromUtf8(datagram.mid(6, idLen));
        if (peerId == m_localPeerId) continue; // Skip local loopback

        QByteArray cipherPayload = datagram.mid(6 + idLen);
        QByteArray plainPayload;

        if (!m_encryptionKey.isEmpty()) {
            if (!CryptoEngine::decryptEnvelope(cipherPayload, m_encryptionKey, plainPayload)) {
                plainPayload = cipherPayload; // Fallback if unencrypted
            }
        } else {
            plainPayload = cipherPayload;
        }

        // Decode audio
        QVector<qint16> outPcm;
        if (codecTag == 'O') {
            if (!m_peerDecoders.contains(peerId)) {
                PeerDecoderState state;
                state.opus = new OpusAudioPipeline(AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, 64000);
                state.opus->initialize();
                m_peerDecoders.insert(peerId, state);
            }
            m_peerDecoders[peerId].opus->decodeFrame(plainPayload, outPcm, OPUS_SAMPLES_PER_FRAME);
        } else if (codecTag == 'A') {
            QVector<qint16> pcm16k;
            if (m_amrManager.decodeFrame(plainPayload, pcm16k) > 0) {
                outPcm = AmrCodecManager::resample16To48(pcm16k.constData(), pcm16k.size());
            }
        }

        if (!outPcm.isEmpty()) {
            // Track peer speaking state
            float sumSq = 0.0f;
            for (qint16 s : outPcm) {
                float f = s / 32768.0f;
                sumSq += f * f;
            }
            float peerRms = std::sqrt(sumSq / outPcm.size());
            bool peerSpeaking = (peerRms > 0.02f);
            if (m_peerDecoders.contains(peerId)) {
                if (m_peerDecoders[peerId].isSpeaking != peerSpeaking) {
                    m_peerDecoders[peerId].isSpeaking = peerSpeaking;
                    emit peerSpeakingStateChanged(peerId, peerSpeaking);
                }
            }

            // Apply Speaker volume multiplier
            if (m_outputVolumeMultiplier != 1.0f) {
                for (int i = 0; i < outPcm.size(); ++i) {
                    float val = outPcm[i] * m_outputVolumeMultiplier;
                    outPcm[i] = static_cast<qint16>(qBound(-32768.0f, val, 32767.0f));
                }
            }

            playDecodedSamples(outPcm.constData(), outPcm.size());
        }
    }
}

void AudioManager::playDecodedSamples(const qint16* samples, int count) {
    if (!m_hWaveOut || !samples || count <= 0) return;

    WAVEHDR* pHdr = new WAVEHDR();
    memset(pHdr, 0, sizeof(WAVEHDR));

    int bytes = count * sizeof(qint16);
    char* buf = new char[bytes];
    memcpy(buf, samples, bytes);

    pHdr->lpData = buf;
    pHdr->dwBufferLength = bytes;
    pHdr->dwFlags = 0;

    waveOutPrepareHeader(m_hWaveOut, pHdr, sizeof(WAVEHDR));
    waveOutWrite(m_hWaveOut, pHdr, sizeof(WAVEHDR));
}

} // namespace Onyx

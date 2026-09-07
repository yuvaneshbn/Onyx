#ifndef PJSIP_MANAGER_H
#define PJSIP_MANAGER_H

#include <QObject>
#include <QString>
#include <pjsua-lib/pjsua.h>
#include "../core/NetworkConfig.h"
#include "../core/JsonProtocol.h"

namespace Onyx {

class PjSipManager : public QObject {
    Q_OBJECT
public:
    enum CallState {
        Idle,
        Calling,
        Incoming,
        Connected,
        Terminated
    };

    explicit PjSipManager(QObject* parent = nullptr);
    explicit PjSipManager(const PeerInfo& localPeer, QObject* parent = nullptr);
    ~PjSipManager() override;

    bool initialize(quint16 sipPort = SIP_PORT);
    void cleanup();

    void setLocalPeer(const PeerInfo& peer) { m_localPeer = peer; }

    bool makeCall(const PeerInfo& targetPeer, bool enableVideo, const QString& audioCodec = "opus", const QString& videoCodec = "vp8");
    bool answerCall(bool enableVideo);
    bool declineCall();
    bool hangupCall();

    CallState callState() const { return m_callState; }
    PeerInfo activePeer() const { return m_activePeer; }
    bool isVideoEnabled() const { return m_videoEnabled; }
    quint16 sipPort() const { return m_sipPort; }

signals:
    void incomingCallReceived(const PeerInfo& caller, bool videoEnabled);
    void callRinging(const PeerInfo& peer);
    void callConnected(const PeerInfo& peer, bool videoEnabled, quint16 remoteAudioPort, quint16 remoteVideoPort);
    void callTerminated(const PeerInfo& peer, const QString& reason);
    void sipError(const QString& errorMsg);

public slots:
    void handleIncomingSignal(const CallSignal& signal, const QString& senderIp);

private:
    void setCallState(CallState state);

    PeerInfo m_localPeer;
    PeerInfo m_activePeer;
    CallState m_callState = Idle;
    quint16 m_sipPort = SIP_PORT;
    bool m_videoEnabled = false;
    bool m_pjsuaInitialized = false;
    pjsua_transport_id m_transportId = -1;
    pjsua_acc_id m_accId = -1;
};

} // namespace Onyx

#endif // PJSIP_MANAGER_H

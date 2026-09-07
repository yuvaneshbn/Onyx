#include "PjSipManager.h"
#include <QDebug>

namespace Onyx {

PjSipManager::PjSipManager(QObject* parent)
    : QObject(parent) {
}

PjSipManager::PjSipManager(const PeerInfo& localPeer, QObject* parent)
    : QObject(parent),
      m_localPeer(localPeer) {
}

PjSipManager::~PjSipManager() {
    cleanup();
}

bool PjSipManager::initialize(quint16 sipPort) {
    m_sipPort = sipPort;

    pj_status_t status = pjsua_create();
    if (status != PJ_SUCCESS) {
        qWarning() << "pjsua_create failed with code:" << status;
        return false;
    }

    pjsua_config cfg;
    pjsua_logging_config logCfg;
    pjsua_media_config medCfg;

    pjsua_config_default(&cfg);
    pjsua_logging_config_default(&logCfg);
    pjsua_media_config_default(&medCfg);

    logCfg.level = 2; // Moderate logging
    logCfg.console_level = 2;

    cfg.user_agent = pj_str(const_cast<char*>("LanVoiceChat/1.0"));

    status = pjsua_init(&cfg, &logCfg, &medCfg);
    if (status != PJ_SUCCESS) {
        qWarning() << "pjsua_init failed with code:" << status;
        pjsua_destroy();
        return false;
    }

    // Create local SIP UDP transport
    pjsua_transport_config tcfg;
    pjsua_transport_config_default(&tcfg);
    tcfg.port = m_sipPort;

    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &tcfg, &m_transportId);
    if (status != PJ_SUCCESS) {
        qWarning() << "pjsua_transport_create on port" << m_sipPort << "failed, trying port 0 (ephemeral)";
        tcfg.port = 0;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &tcfg, &m_transportId);
        if (status != PJ_SUCCESS) {
            qWarning() << "pjsua_transport_create failed completely with code:" << status;
            pjsua_destroy();
            return false;
        }
    }

    status = pjsua_start();
    if (status != PJ_SUCCESS) {
        qWarning() << "pjsua_start failed with code:" << status;
        pjsua_destroy();
        return false;
    }

    // Add local SIP account
    pjsua_acc_add_local(m_transportId, PJ_TRUE, &m_accId);

    m_pjsuaInitialized = true;
    qDebug() << "PjSipManager initialized successfully on SIP port:" << m_sipPort;
    return true;
}

void PjSipManager::cleanup() {
    if (m_pjsuaInitialized) {
        pjsua_destroy();
        m_pjsuaInitialized = false;
        m_transportId = -1;
        m_accId = -1;
    }
    setCallState(Idle);
}

void PjSipManager::setCallState(CallState state) {
    m_callState = state;
}

bool PjSipManager::makeCall(const PeerInfo& targetPeer, bool enableVideo, const QString& audioCodec, const QString& videoCodec) {
    if (m_callState != Idle) {
        emit sipError("A call is already in progress");
        return false;
    }

    m_activePeer = targetPeer;
    m_videoEnabled = enableVideo;
    setCallState(Calling);

    emit callRinging(targetPeer);
    return true;
}

bool PjSipManager::answerCall(bool enableVideo) {
    if (m_callState != Incoming) {
        return false;
    }

    m_videoEnabled = enableVideo;
    setCallState(Connected);

    emit callConnected(m_activePeer, m_videoEnabled, AUDIO_RTP_PORT_START, VIDEO_RTP_PORT_START);
    return true;
}

bool PjSipManager::declineCall() {
    if (m_callState != Incoming) {
        return false;
    }

    PeerInfo peer = m_activePeer;
    setCallState(Idle);
    m_activePeer = PeerInfo();

    emit callTerminated(peer, "Call declined");
    return true;
}

bool PjSipManager::hangupCall() {
    if (m_callState == Idle) {
        return false;
    }

    PeerInfo peer = m_activePeer;
    setCallState(Idle);
    m_activePeer = PeerInfo();

    emit callTerminated(peer, "Call ended by local user");
    return true;
}

void PjSipManager::handleIncomingSignal(const CallSignal& signal, const QString& senderIp) {
    switch (signal.type) {
        case CallSignal::Invite: {
            if (m_callState != Idle) {
                // Busy
                return;
            }
            m_activePeer.peerId = signal.callerId;
            m_activePeer.username = signal.callerName;
            m_activePeer.ipAddress = senderIp;
            m_videoEnabled = signal.videoEnabled;
            setCallState(Incoming);
            emit incomingCallReceived(m_activePeer, m_videoEnabled);
            break;
        }
        case CallSignal::Ringing: {
            if (m_callState == Calling) {
                emit callRinging(m_activePeer);
            }
            break;
        }
        case CallSignal::Accept: {
            if (m_callState == Calling) {
                setCallState(Connected);
                m_videoEnabled = signal.videoEnabled;
                emit callConnected(m_activePeer, m_videoEnabled, signal.audioPort, signal.videoPort);
            }
            break;
        }
        case CallSignal::Decline: {
            if (m_callState == Calling) {
                PeerInfo peer = m_activePeer;
                setCallState(Idle);
                m_activePeer = PeerInfo();
                emit callTerminated(peer, "Remote peer declined the call");
            }
            break;
        }
        case CallSignal::Hangup: {
            if (m_callState != Idle) {
                PeerInfo peer = m_activePeer;
                setCallState(Idle);
                m_activePeer = PeerInfo();
                emit callTerminated(peer, "Call ended by remote peer");
            }
            break;
        }
    }
}

} // namespace Onyx

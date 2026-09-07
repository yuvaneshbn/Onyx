#include "P2PDiscovery.h"
#include <QNetworkInterface>
#include <QDebug>

namespace Onyx {

P2PDiscovery::P2PDiscovery(const PeerInfo& localPeer, QObject* parent)
    : QObject(parent),
      m_localPeer(localPeer),
      m_multicastAddress(MULTICAST_GROUP) {
    // If local IP is empty, determine first valid non-loopback IPv4 address
    if (m_localPeer.ipAddress.isEmpty()) {
        const auto addresses = QNetworkInterface::allAddresses();
        for (const auto& addr : addresses) {
            if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
                m_localPeer.ipAddress = addr.toString();
                break;
            }
        }
        if (m_localPeer.ipAddress.isEmpty()) {
            m_localPeer.ipAddress = "127.0.0.1";
        }
    }
}

P2PDiscovery::~P2PDiscovery() {
    stop();
}

bool P2PDiscovery::start() {
    if (m_udpSocket) {
        return true;
    }

    m_udpSocket = new QUdpSocket(this);

    // Bind with ShareAddress and ReuseAddressHint so multiple local instances can run
    bool bound = m_udpSocket->bind(QHostAddress::AnyIPv4, DISCOVERY_PORT,
                                   QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!bound) {
        emit discoveryError("Failed to bind UDP multicast discovery socket to port " + QString::number(DISCOVERY_PORT));
        delete m_udpSocket;
        m_udpSocket = nullptr;
        return false;
    }

    if (!m_udpSocket->joinMulticastGroup(m_multicastAddress)) {
        qWarning() << "Could not join multicast group:" << m_multicastAddress.toString();
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &P2PDiscovery::onReadyRead);

    // Beacon broadcast timer
    m_beaconTimer = new QTimer(this);
    connect(m_beaconTimer, &QTimer::timeout, this, &P2PDiscovery::sendBeacon);
    m_beaconTimer->start(BEACON_INTERVAL_MS);

    // Peer timeout checker timer
    m_cleanupTimer = new QTimer(this);
    connect(m_cleanupTimer, &QTimer::timeout, this, &P2PDiscovery::checkPeerTimeouts);
    m_cleanupTimer->start(2000);

    // Broadcast initial beacon immediately
    sendBeacon();
    return true;
}

void P2PDiscovery::stop() {
    if (m_beaconTimer) {
        m_beaconTimer->stop();
        delete m_beaconTimer;
        m_beaconTimer = nullptr;
    }
    if (m_cleanupTimer) {
        m_cleanupTimer->stop();
        delete m_cleanupTimer;
        m_cleanupTimer = nullptr;
    }
    if (m_udpSocket) {
        m_udpSocket->leaveMulticastGroup(m_multicastAddress);
        m_udpSocket->close();
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }
    m_peers.clear();
}

void P2PDiscovery::updateLocalProfile(const QString& username, const QString& statusMessage, const QString& avatarColor) {
    m_localPeer.username = username;
    m_localPeer.statusMessage = statusMessage;
    m_localPeer.avatarColor = avatarColor;
    sendBeacon();
}

void P2PDiscovery::updateLocalStatus(const QString& status) {
    m_localPeer.status = status;
    sendBeacon();
}

void P2PDiscovery::updateLocalCodecs(const QString& audioCodec, const QString& videoCodec) {
    m_localPeer.audioCodec = audioCodec;
    m_localPeer.videoCodec = videoCodec;
    sendBeacon();
}

void P2PDiscovery::updateCallState(bool inCall, bool micMuted, bool videoMuted, bool isSpeaking) {
    m_localPeer.inCall = inCall;
    m_localPeer.micMuted = micMuted;
    m_localPeer.videoMuted = videoMuted;
    m_localPeer.isSpeaking = isSpeaking;
    sendBeacon();
}

void P2PDiscovery::broadcastCallSignal(const CallSignal& signal) {
    if (!m_udpSocket) return;
    QByteArray data = JsonProtocol::createCallSignal(signal);
    m_udpSocket->writeDatagram(data, m_multicastAddress, DISCOVERY_PORT);
}

void P2PDiscovery::sendDirectCallSignal(const CallSignal& signal, const QString& targetIp) {
    if (!m_udpSocket) return;
    QByteArray data = JsonProtocol::createCallSignal(signal);
    m_udpSocket->writeDatagram(data, QHostAddress(targetIp), DISCOVERY_PORT);
}

QList<PeerInfo> P2PDiscovery::discoveredPeers() const {
    return m_peers.values();
}

bool P2PDiscovery::findPeer(const QString& peerId, PeerInfo& outPeer) const {
    if (m_peers.contains(peerId)) {
        outPeer = m_peers.value(peerId);
        return true;
    }
    return false;
}

void P2PDiscovery::sendBeacon() {
    if (!m_udpSocket) return;

    QByteArray datagram = JsonProtocol::createDiscoveryBeacon(m_localPeer);
    m_udpSocket->writeDatagram(datagram, m_multicastAddress, DISCOVERY_PORT);
}

void P2PDiscovery::onReadyRead() {
    while (m_udpSocket && m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_udpSocket->pendingDatagramSize()));
        QHostAddress senderAddress;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QString senderIp = senderAddress.toString();
        if (senderIp.startsWith("::ffff:")) senderIp = senderIp.mid(7);

        // Check if this is a call signal datagram
        CallSignal callSig;
        if (JsonProtocol::parseCallSignal(datagram, callSig)) {
            if (callSig.callerId != m_localPeer.peerId) {
                emit callSignalReceived(callSig, senderIp);
            }
            continue;
        }

        PeerInfo peer;
        if (!JsonProtocol::parseDiscoveryBeacon(datagram, peer)) {
            continue;
        }

        // Ignore our own beacon
        if (peer.peerId == m_localPeer.peerId) {
            continue;
        }

        // If IP is reported as loopback or empty, use sender socket IP
        if (peer.ipAddress.isEmpty() || peer.ipAddress == "127.0.0.1") {
            if (senderIp != "127.0.0.1" && !senderIp.isEmpty()) {
                peer.ipAddress = senderIp;
            }
        }

        if (!m_peers.contains(peer.peerId)) {
            m_peers.insert(peer.peerId, peer);
            emit peerDiscovered(peer);
        } else {
            m_peers[peer.peerId] = peer;
            emit peerUpdated(peer);
        }
    }
}

void P2PDiscovery::checkPeerTimeouts() {
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto it = m_peers.begin();
    while (it != m_peers.end()) {
        if (now - it.value().lastSeenMs > PEER_TIMEOUT_MS) {
            QString deadPeerId = it.key();
            it = m_peers.erase(it);
            emit peerLost(deadPeerId);
        } else {
            ++it;
        }
    }
}

} // namespace Onyx

#ifndef P2P_DISCOVERY_H
#define P2P_DISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QMap>
#include <QHostAddress>
#include "JsonProtocol.h"
#include "NetworkConfig.h"

namespace Onyx {

class P2PDiscovery : public QObject {
    Q_OBJECT
public:
    explicit P2PDiscovery(const PeerInfo& localPeer, QObject* parent = nullptr);
    ~P2PDiscovery() override;

    bool start();
    void stop();

    void updateLocalProfile(const QString& username, const QString& statusMessage, const QString& avatarColor);
    void updateLocalStatus(const QString& status);
    void updateLocalCodecs(const QString& audioCodec, const QString& videoCodec);
    void updateCallState(bool inCall, bool micMuted, bool videoMuted, bool isSpeaking);

    void broadcastCallSignal(const CallSignal& signal);
    void sendDirectCallSignal(const CallSignal& signal, const QString& targetIp);

    PeerInfo localPeer() const { return m_localPeer; }
    QList<PeerInfo> discoveredPeers() const;
    bool findPeer(const QString& peerId, PeerInfo& outPeer) const;

signals:
    void peerDiscovered(const Onyx::PeerInfo& peer);
    void peerUpdated(const Onyx::PeerInfo& peer);
    void peerLost(const QString& peerId);
    void callSignalReceived(const Onyx::CallSignal& signal, const QString& senderIp);
    void discoveryError(const QString& errorMsg);

private slots:
    void onReadyRead();
    void sendBeacon();
    void checkPeerTimeouts();

private:
    PeerInfo m_localPeer;
    QUdpSocket* m_udpSocket = nullptr;
    QTimer* m_beaconTimer = nullptr;
    QTimer* m_cleanupTimer = nullptr;
    QMap<QString, PeerInfo> m_peers;
    QHostAddress m_multicastAddress;
};

} // namespace Onyx

#endif // P2P_DISCOVERY_H

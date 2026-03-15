#include "ice_agent.h"
#include <QNetworkInterface>
#include <QDebug>

namespace network {

IceAgent::IceAgent(QUdpSocket* socket, QObject* parent)
    : QObject(parent), socket_(socket) {
    stunClient_ = new StunClient(socket, this);
    
    connect(stunClient_, &StunClient::publicEndpointResolved, this, &IceAgent::onStunResolved);
    connect(stunClient_, &StunClient::resolveFailed, this, &IceAgent::onStunFailed);
}

IceAgent::~IceAgent() = default;

void IceAgent::gatherHostCandidates() {
    uint16_t localPort = socket_->localPort();
    
    // Iterate over all active network interfaces
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (!iface.flags().testFlag(QNetworkInterface::IsUp) || 
            iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            continue;
        }

        const auto entries = iface.addressEntries();
        for (const QNetworkAddressEntry& entry : entries) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                Candidate c;
                c.type = CandidateType::Host;
                c.ip = entry.ip();
                c.port = localPort;
                c.foundation = "host_1";
                c.priority = calculatePriority(CandidateType::Host, 1);
                
                localCandidates_.push_back(c);
            }
        }
    }
}

uint32_t IceAgent::calculatePriority(CandidateType type, uint16_t componentId) {
    // RFC 5245 Priority Formula
    // priority = (2^24)*(type preference) + (2^8)*(local preference) + (256 - component ID)
    uint32_t typePref = 0;
    switch (type) {
        case CandidateType::Host: typePref = 126; break;
        case CandidateType::ServerReflexive: typePref = 100; break;
        case CandidateType::Relayed: typePref = 0; break;
    }
    
    uint32_t localPref = 65535; // Max local preference
    return (typePref << 24) + (localPref << 8) + (256 - componentId);
}

void IceAgent::gatherCandidates(const QString& stunServerHost, uint16_t stunServerPort) {
    localCandidates_.clear();
    gatherHostCandidates();

    if (!stunServerHost.isEmpty()) {
        stunClient_->resolve(stunServerHost, stunServerPort);
    } else {
        // No STUN requested, gathering is already complete (LAN only)
        emit gatheringComplete(localCandidates_);
    }
}

void IceAgent::onStunResolved(const QHostAddress& ip, uint16_t port) {
    Candidate c;
    c.type = CandidateType::ServerReflexive;
    c.ip = ip;
    c.port = port;
    c.foundation = "srflx_1";
    c.priority = calculatePriority(CandidateType::ServerReflexive, 1);
    
    localCandidates_.push_back(c);
    
    qInfo() << "Gathered STUN candidate:" << ip.toString() << ":" << port;
    emit gatheringComplete(localCandidates_);
}

void IceAgent::onStunFailed(const QString& reason) {
    qWarning() << "STUN gathering failed:" << reason << ". Falling back to HOST candidates only.";
    emit gatheringComplete(localCandidates_);
}

} // namespace network

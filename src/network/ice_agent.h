#ifndef ICE_AGENT_H
#define ICE_AGENT_H

#pragma once

#include "stun.h"
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QString>
#include <vector>

namespace network {

// Simplified Interactive Connectivity Establishment (ICE).
// Gathers Host (local IPs) and Server Reflexive (STUN public IP) candidates.
// Normally you exchange these with a peer via SDP signaling.
class IceAgent : public QObject {
    Q_OBJECT

public:
    enum class CandidateType {
        Host,
        ServerReflexive,
        Relayed // (TURN - unimplemented here)
    };

    struct Candidate {
        CandidateType type;
        QHostAddress ip;
        uint16_t port;
        QString foundation;
        uint32_t priority;
    };

    explicit IceAgent(QUdpSocket* socket, QObject* parent = nullptr);
    ~IceAgent() override;

    void gatherCandidates(const QString& stunServerHost, uint16_t stunServerPort = 3478);
    const std::vector<Candidate>& getLocalCandidates() const { return localCandidates_; }

signals:
    // Emitted when gathering completes
    void gatheringComplete(const std::vector<Candidate>& candidates);
    void gatheringFailed(const QString& reason);

private slots:
    void onStunResolved(const QHostAddress& ip, uint16_t port);
    void onStunFailed(const QString& reason);

private:
    QUdpSocket* socket_{nullptr};
    StunClient* stunClient_{nullptr};
    std::vector<Candidate> localCandidates_;

    void gatherHostCandidates();
    uint32_t calculatePriority(CandidateType type, uint16_t componentId);
};

} // namespace network

#endif // ICE_AGENT_H

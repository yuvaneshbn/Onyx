#ifndef SUBSCRIPTION_TABLE_H
#define SUBSCRIPTION_TABLE_H

#pragma once

#include <QObject>
#include <QHostAddress>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <vector>

namespace network {

// Forwarding logic core.
// Keeps track of which Child node wants to listen to which Sender ID.
// This allows the Router to drop packets at the edge rather than broadcasting everything.
class SubscriptionTable : public QObject {
    Q_OBJECT

public:
    struct Endpoint {
        QHostAddress ip;
        uint16_t port;
        bool operator==(const Endpoint& other) const {
            return ip == other.ip && port == other.port;
        }
    };

    struct EndpointHash {
        std::size_t operator()(const Endpoint& p) const {
            return qHash(p.ip.toString() + ":" + QString::number(p.port));
        }
    };

    explicit SubscriptionTable(QObject* parent = nullptr);

    // Register that `listener` wants to receive audio from `targetSsrc`
    void addSubscription(uint32_t targetSsrc, const Endpoint& listener);
    void removeSubscription(uint32_t targetSsrc, const Endpoint& listener);
    
    // Caller removes a dead child from all tables
    void removeEndpoint(const Endpoint& listener);

    // Returns list of endpoints that need this packet. 
    std::vector<Endpoint> getSubscribersFor(uint32_t ssrc);

private:
    std::mutex mutex_;
    // Map of Originating SSRC -> List of IPs that are subscribed
    std::unordered_map<uint32_t, std::unordered_set<Endpoint, EndpointHash>> subscriptions_;
};

} // namespace network

#endif // SUBSCRIPTION_TABLE_H

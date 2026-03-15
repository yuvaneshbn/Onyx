#include "subscription_table.h"

namespace network {

SubscriptionTable::SubscriptionTable(QObject* parent) : QObject(parent) {}

void SubscriptionTable::addSubscription(uint32_t targetSsrc, const Endpoint& listener) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscriptions_[targetSsrc].insert(listener);
}

void SubscriptionTable::removeSubscription(uint32_t targetSsrc, const Endpoint& listener) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = subscriptions_.find(targetSsrc);
    if (it != subscriptions_.end()) {
        it->second.erase(listener);
        if (it->second.empty()) {
            subscriptions_.erase(it);
        }
    }
}

void SubscriptionTable::removeEndpoint(const Endpoint& listener) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto it = subscriptions_.begin(); it != subscriptions_.end(); ) {
        it->second.erase(listener);
        if (it->second.empty()) {
            it = subscriptions_.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<SubscriptionTable::Endpoint> SubscriptionTable::getSubscribersFor(uint32_t ssrc) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Endpoint> result;
    
    auto it = subscriptions_.find(ssrc);
    if (it != subscriptions_.end()) {
        result.assign(it->second.begin(), it->second.end());
    }
    
    return result;
}

} // namespace network

#ifndef SCHEDULER_H
#define SCHEDULER_H

#pragma once

#include <QObject>
#include <QTimer>
#include <functional>
#include <memory>
#include <unordered_map>

namespace core {

class Scheduler : public QObject {
    Q_OBJECT

public:
    static Scheduler& instance() {
        static Scheduler instance;
        return instance;
    }

    // Schedule a repeating task (e.g., heartbeats every 15s)
    uint32_t scheduleRepeating(int intervalMs, std::function<void()> task) {
        uint32_t id = nextId_++;
        auto timer = std::make_unique<QTimer>(this);
        connect(timer.get(), &QTimer::timeout, this, task);
        timer->start(intervalMs);
        timers_[id] = std::move(timer);
        return id;
    }

    // Cancel a scheduled task
    void cancel(uint32_t id) {
        timers_.erase(id);
    }

private:
    Scheduler() = default;
    
    uint32_t nextId_{1};
    std::unordered_map<uint32_t, std::unique_ptr<QTimer>> timers_;
};

} // namespace core

#endif // SCHEDULER_H

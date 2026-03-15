#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#pragma once

#include <QObject>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <any>

namespace core {

// Simple, thread-safe asynchronous Event Bus utilizing Qt's signal/slot system 
// to decouple GUI from Audio/Network threads.
class EventBus : public QObject {
    Q_OBJECT

public:
    static EventBus& instance() {
        static EventBus instance;
        return instance;
    }

    template<typename T>
    void publish(const std::string& topic, const T& payload) {
        emit eventFired(QString::fromStdString(topic), QVariant::fromValue(payload));
    }

signals:
    // Core signal that Qt will route across threads if necessary
    void eventFired(const QString& topic, const QVariant& payload);

private:
    EventBus() = default;
    ~EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
};

} // namespace core

#endif // EVENT_BUS_H

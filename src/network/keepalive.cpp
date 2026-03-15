#include "keepalive.h"
#include <QDebug>

namespace network {

KeepAlive::KeepAlive(QUdpSocket* socket, QObject* parent)
    : QObject(parent), socket_(socket) {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &KeepAlive::sendPings);
}

KeepAlive::~KeepAlive() {
    stop();
}

void KeepAlive::start(int intervalMs) {
    timer_->start(intervalMs);
}

void KeepAlive::stop() {
    timer_->stop();
}

void KeepAlive::addTarget(const QHostAddress& ip, uint16_t port) {
    targets_.insert({ip, port});
}

void KeepAlive::removeTarget(const QHostAddress& ip, uint16_t port) {
    targets_.erase({ip, port});
}

void KeepAlive::sendPings() {
    if (!socket_ || socket_->state() != QUdpSocket::BoundState) return;

    // A simple 1-byte payload is enough to refresh NAT tables
    QByteArray ping(1, '\0');

    for (const auto& target : targets_) {
        socket_->writeDatagram(ping, target.ip, target.port);
    }
}

} // namespace network

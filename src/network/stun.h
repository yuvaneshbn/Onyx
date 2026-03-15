#ifndef STUN_H
#define STUN_H

#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>

namespace network {

// Basic STUN Client implementation (RFC 5389 subset).
// Resolves the public IP and Port of this client by querying a public STUN server.
class StunClient : public QObject {
    Q_OBJECT

public:
    explicit StunClient(QUdpSocket* socket, QObject* parent = nullptr);
    ~StunClient();

    void resolve(const QString& stunServerHost, uint16_t stunServerPort = 3478);

signals:
    void publicEndpointResolved(const QHostAddress& publicIp, uint16_t publicPort);
    void resolveFailed(const QString& reason);

private slots:
    void onReadyRead();
    void onTimeout();

private:
    QUdpSocket* socket_{nullptr};
    QTimer* timeoutTimer_{nullptr};
    QByteArray transactionId_;
    
    QHostAddress stunIp_;
    uint16_t stunPort_{3478};
    int retryCount_{0};

    void sendBindingRequest();
    QByteArray generateTransactionId();
};

} // namespace network

#endif // STUN_H

#include "stun.h"
#include <QNetworkDatagram>
#include <QRandomGenerator>
#include <QDebug>

namespace network {

StunClient::StunClient(QUdpSocket* socket, QObject* parent)
    : QObject(parent), socket_(socket) {
    
    timeoutTimer_ = new QTimer(this);
    timeoutTimer_->setSingleShot(true);
    
    connect(socket_, &QUdpSocket::readyRead, this, &StunClient::onReadyRead);
    connect(timeoutTimer_, &QTimer::timeout, this, &StunClient::onTimeout);
}

StunClient::~StunClient() {
    timeoutTimer_->stop();
}

void StunClient::resolve(const QString& stunServerHost, uint16_t stunServerPort) {
    stunIp_ = QHostAddress(stunServerHost);
    stunPort_ = stunServerPort;
    retryCount_ = 0;
    
    transactionId_ = generateTransactionId();
    sendBindingRequest();
}

void StunClient::sendBindingRequest() {
    if (retryCount_ > 3) {
        emit resolveFailed("STUN resolution timed out after 3 retries.");
        return;
    }

    QByteArray request;
    request.append(static_cast<char>(0x00)); // STUN Message Type: Binding Request
    request.append(static_cast<char>(0x01)); 
    request.append(static_cast<char>(0x00)); // Message Length: 0
    request.append(static_cast<char>(0x00));
    
    // Magic Cookie (RFC 5389)
    request.append(static_cast<char>(0x21));
    request.append(static_cast<char>(0x12));
    request.append(static_cast<char>(0xA4));
    request.append(static_cast<char>(0x42));

    // Transaction ID (12 bytes)
    request.append(transactionId_);

    socket_->writeDatagram(request, stunIp_, stunPort_);
    
    timeoutTimer_->start(500 * (1 << retryCount_)); // Exponential backoff
    retryCount_++;
}

QByteArray StunClient::generateTransactionId() {
    QByteArray id;
    id.resize(12);
    for (int i = 0; i < 12; ++i) {
        id[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
    return id;
}

void StunClient::onReadyRead() {
    while (socket_->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket_->receiveDatagram();
        
        // We only care about STUN responses from our target server
        if (datagram.senderAddress() != stunIp_ || datagram.senderPort() != stunPort_) {
            continue;
        }

        QByteArray data = datagram.data();
        if (data.size() < 20) continue;

        // Check Transaction ID matches
        if (data.mid(8, 12) != transactionId_) continue;

        // Message Type: Binding Response is 0x0101
        uint16_t msgType = (static_cast<uint8_t>(data[0]) << 8) | static_cast<uint8_t>(data[1]);
        if (msgType == 0x0101) {
            timeoutTimer_->stop();

            // Very simplified parsing: look for XOR-MAPPED-ADDRESS attribute (0x0020)
            int offset = 20;
            while (offset + 4 <= data.size()) {
                uint16_t attrType = (static_cast<uint8_t>(data[offset]) << 8) | static_cast<uint8_t>(data[offset+1]);
                uint16_t attrLen = (static_cast<uint8_t>(data[offset+2]) << 8) | static_cast<uint8_t>(data[offset+3]);
                
                if (attrType == 0x0020 && offset + 4 + attrLen <= data.size()) { // XOR-MAPPED-ADDRESS
                    // Skip 'reserved' byte and 'family' byte
                    // Read Port (XORed with top 16 bits of magic cookie)
                    uint16_t xorPort = (static_cast<uint8_t>(data[offset+4+2]) << 8) | static_cast<uint8_t>(data[offset+4+3]);
                    uint16_t publicPort = xorPort ^ 0x2112;

                    // Read IP (XORed with magic cookie)
                    uint32_t xorIp = (static_cast<uint8_t>(data[offset+4+4]) << 24) |
                                     (static_cast<uint8_t>(data[offset+4+5]) << 16) |
                                     (static_cast<uint8_t>(data[offset+4+6]) << 8) |
                                      static_cast<uint8_t>(data[offset+4+7]);
                    uint32_t publicIp = xorIp ^ 0x2112A442;

                    emit publicEndpointResolved(QHostAddress(publicIp), publicPort);
                    return;
                }
                
                offset += 4 + attrLen; // Skip to next attribute
            }
        }
    }
}

void StunClient::onTimeout() {
    sendBindingRequest();
}

} // namespace network

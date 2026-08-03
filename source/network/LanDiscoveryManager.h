// LanDiscoveryManager.h
#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QNetworkInterface>

class LanDiscoveryManager : public QObject {
    Q_OBJECT
public:
    explicit LanDiscoveryManager(QObject *parent = nullptr) 
        : QObject(parent), m_multicastAddress("239.255.43.21"), m_port(43210) {
        
        m_socket = new QUdpSocket(this);
        // Bind multicast listener with port reuse enabled
        m_socket->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        m_socket->joinMulticastGroup(m_multicastAddress);

        connect(m_socket, &QUdpSocket::readyRead, this, &LanDiscoveryManager::processIncomingBeacons);

        // Periodically broadcast peer availability
        m_beaconTimer = new QTimer(this);
        connect(m_beaconTimer, &QTimer::timeout, this, &LanDiscoveryManager::sendPresenceBeacon);
        m_beaconTimer->start(5000); // Trigger beacon sweep every 5 seconds
    }

signals:
    void peerDiscovered(const QString &name, const QString &sipUri);

private:
    void sendPresenceBeacon() {
        QString localIp = getLocalIpAddress();
        if (localIp.isEmpty()) return;

        // Build beacon payload containing peer name and destination URI
        QString beaconMessage = QString("PEER_DISCOVERY:Name=User-%1;URI=sip:%2@%3")
                                   .arg(localIp.split('.').last())
                                   .arg("user")
                                   .arg(localIp);
        
        QByteArray datagram = beaconMessage.toUtf8();
        m_socket->writeDatagram(datagram, m_multicastAddress, m_port);
    }

    void processIncomingBeacons() {
        while (m_socket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(m_socket->pendingDatagramSize());
            QHostAddress senderIp;
            m_socket->readDatagram(datagram.data(), datagram.size(), &senderIp);

            QString msg = QString::fromUtf8(datagram).trimmed();
            if (msg.startsWith("PEER_DISCOVERY:")) {
                QString body = msg.mid(15);
                QStringList parts = body.split(';');
                if (parts.size() == 2) {
                    QString name = parts[0].split('=').last();
                    QString sipUri = parts[1].split('=').last();
                    
                    // Filter out local beacons
                    if (!sipUri.contains(getLocalIpAddress())) {
                        emit peerDiscovered(name, sipUri);
                    }
                }
            }
        }
    }

    QString getLocalIpAddress() {
        for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
                return address.toString();
            }
        }
        return "";
    }

    QHostAddress m_multicastAddress;
    quint16 m_port;
    QUdpSocket *m_socket;
    QTimer *m_beaconTimer;
};
#pragma once

#include <QObject>
#include <QHostAddress>
#include <QStringList>
#include <QTcpSocket>
#include <QUdpSocket>

class PortKnocker : public QObject
{
    Q_OBJECT
public:
    explicit PortKnocker(QObject *parent = nullptr) : QObject(parent) {}

    void executeKnockSequence(const QString &host, const QString &portsCsv)
    {
        if (portsCsv.isEmpty())
            return;

        QStringList ports = portsCsv.split(',', Qt::SkipEmptyParts);
        for (const QString &portStr : ports) {
            bool ok;
            quint16 port = portStr.toUShort(&ok);
            if (ok) {
                QUdpSocket socket;
                // Send an empty payload to trigger firewall whitelisting
                QByteArray emptyPayload;
                socket.writeDatagram(emptyPayload, QHostAddress(host), port);
            }
        }
    }
};

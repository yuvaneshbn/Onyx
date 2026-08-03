#pragma once

#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class FileTransferManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool isTransferring READ isTransferring NOTIFY statusChanged)

public:
    explicit FileTransferManager(quint16 port, QObject *parent = nullptr)
        : QObject(parent),
          m_port(port),
          m_server(new QTcpServer(this)),
          m_progress(0),
          m_transferring(false)
    {
        connect(m_server, &QTcpServer::newConnection, this, &FileTransferManager::handleIncomingConnection);
        m_server->listen(QHostAddress::Any, m_port);
    }

    Q_INVOKABLE void sendFile(const QString &targetIp, const QString &filePath)
    {
        if (m_transferring) {
            return;
        }

        auto *file = new QFile(filePath, this);
        if (!file->open(QIODevice::ReadOnly)) {
            file->deleteLater();
            return;
        }

        m_transferring = true;
        m_progress = 0;
        emit statusChanged();
        emit progressChanged();

        auto *socket = new QTcpSocket(this);
        socket->connectToHost(targetIp, m_port);

        connect(socket, &QTcpSocket::connected, this, [this, socket, file]() {
            const QFileInfo fileInfo(*file);
            const QString header =
                QStringLiteral("METADATA:Name=%1;Size=%2\n").arg(fileInfo.fileName()).arg(fileInfo.size());
            socket->write(header.toUtf8());

            auto streamData = [this, socket, file]() {
                while (socket->bytesToWrite() < 65536 && !file->atEnd()) {
                    socket->write(file->read(32768));
                }
                if (file->atEnd() && socket->bytesToWrite() == 0) {
                    socket->disconnectFromHost();
                }
            };

            connect(socket, &QTcpSocket::bytesWritten, this, streamData);
            streamData();
        });

        connect(socket, &QTcpSocket::disconnected, this, [this, socket, file]() {
            file->close();
            file->deleteLater();
            socket->deleteLater();
            m_transferring = false;
            m_progress = 100;
            emit progressChanged();
            emit statusChanged();
        });
    }

    int progress() const { return m_progress; }
    bool isTransferring() const { return m_transferring; }

signals:
    void progressChanged();
    void statusChanged();
    void fileReceived(const QString &fileName, const QString &savedPath);

private:
    struct ReceiveState {
        bool parsedHeader = false;
        qint64 totalSize = 0;
        qint64 bytesWritten = 0;
        QString name;
    };

    static QString extractHeaderValue(const QStringList &parts, const QString &key)
    {
        for (const QString &part : parts) {
            if (part.startsWith(key)) {
                return part.section('=', 1);
            }
        }
        return {};
    }

    void handleIncomingConnection()
    {
        QTcpSocket *socket = m_server->nextPendingConnection();
        auto *file = new QFile(this);
        auto *state = new ReceiveState();

        connect(socket, &QTcpSocket::readyRead, this, [this, socket, file, state]() {
            if (!state->parsedHeader) {
                const QByteArray line = socket->readLine();
                const QString header = QString::fromUtf8(line).trimmed();
                if (!header.startsWith(QStringLiteral("METADATA:"))) {
                    return;
                }

                const QStringList parts = header.mid(9).split(';', Qt::SkipEmptyParts);
                state->name = extractHeaderValue(parts, QStringLiteral("Name="));
                state->totalSize = extractHeaderValue(parts, QStringLiteral("Size=")).toLongLong();
                if (state->name.isEmpty() || state->totalSize <= 0) {
                    return;
                }

                file->setFileName(QStringLiteral("Received_%1").arg(state->name));
                if (!file->open(QIODevice::WriteOnly)) {
                    return;
                }

                state->parsedHeader = true;
                m_transferring = true;
                m_progress = 0;
                emit statusChanged();
                emit progressChanged();
                return;
            }

            const QByteArray chunk = socket->readAll();
            file->write(chunk);
            state->bytesWritten += chunk.size();
            if (state->totalSize > 0) {
                m_progress = static_cast<int>((state->bytesWritten * 100) / state->totalSize);
                emit progressChanged();
            }
        });

        connect(socket, &QTcpSocket::disconnected, this, [this, socket, file, state]() {
            if (file->isOpen()) {
                file->close();
            }
            m_transferring = false;
            emit statusChanged();
            emit fileReceived(state->name, file->fileName());
            socket->deleteLater();
            delete state;
        });
    }

    quint16 m_port;
    QTcpServer *m_server;
    int m_progress;
    bool m_transferring;
};

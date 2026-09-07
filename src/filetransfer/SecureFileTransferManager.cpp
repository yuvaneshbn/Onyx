#include "SecureFileTransferManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QUuid>
#include <QDataStream>
#include <QDebug>

namespace Onyx {

SecureFileTransferManager::SecureFileTransferManager(QObject* parent)
    : QObject(parent) {
    m_downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (m_downloadDir.isEmpty()) {
        m_downloadDir = QDir::currentPath() + "/downloads";
    }
    QDir().mkpath(m_downloadDir);
}

SecureFileTransferManager::SecureFileTransferManager(const QString& downloadDir, QObject* parent)
    : QObject(parent),
      m_downloadDir(downloadDir) {
    if (m_downloadDir.isEmpty()) {
        m_downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        if (m_downloadDir.isEmpty()) {
            m_downloadDir = QDir::currentPath() + "/downloads";
        }
    }
    QDir().mkpath(m_downloadDir);
}

SecureFileTransferManager::~SecureFileTransferManager() {
    stopServer();
    qDeleteAll(m_transfers);
    m_transfers.clear();
}

bool SecureFileTransferManager::startServer(quint16 port) {
    stopServer();

    m_serverPort = port;
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &SecureFileTransferManager::onNewServerConnection);

    if (!m_tcpServer->listen(QHostAddress::AnyIPv4, m_serverPort)) {
        qWarning() << "Failed to start file transfer TCP server on port:" << m_serverPort;
        delete m_tcpServer;
        m_tcpServer = nullptr;
        return false;
    }

    return true;
}

void SecureFileTransferManager::stopServer() {
    if (m_tcpServer) {
        m_tcpServer->close();
        delete m_tcpServer;
        m_tcpServer = nullptr;
    }
}

void SecureFileTransferManager::setDownloadDirectory(const QString& path) {
    m_downloadDir = path;
    QDir().mkpath(m_downloadDir);
}

QString SecureFileTransferManager::sendFile(const QString& filePath, const PeerInfo& targetPeer) {
    QFileInfo fi(filePath);
    if (!fi.exists() || !fi.isFile()) {
        emit transferFailed(QString(), "Selected file does not exist");
        return QString();
    }

    QString transferId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString sha256 = CryptoEngine::computeFileSha256(filePath);

    ActiveTransfer* transfer = new ActiveTransfer();
    transfer->transferId = transferId;
    transfer->direction = ActiveTransfer::Send;
    transfer->peerId = targetPeer.peerId;
    transfer->peerName = targetPeer.username;
    transfer->peerIp = targetPeer.ipAddress;
    transfer->localFilePath = filePath;
    transfer->fileName = fi.fileName();
    transfer->totalBytes = fi.size();
    transfer->transferredBytes = 0;
    transfer->sha256Hex = sha256;
    transfer->status = ActiveTransfer::InProgress;
    transfer->file = new QFile(filePath);

    if (!transfer->file->open(QIODevice::ReadOnly)) {
        emit transferFailed(transferId, "Failed to open source file for reading");
        delete transfer->file;
        delete transfer;
        return QString();
    }

    m_transfers.insert(transferId, transfer);

    // Connect to peer's file transfer TCP port
    QTcpSocket* socket = new QTcpSocket(this);
    transfer->socket = socket;
    m_socketToTransferId.insert(socket, transferId);

    connect(socket, &QTcpSocket::connected, this, [this, transferId]() {
        if (!m_transfers.contains(transferId)) return;
        ActiveTransfer* t = m_transfers.value(transferId);
        t->timer.start();

        // Send metadata header: Magic, transferId, fileName, fileSize, sha256
        QByteArray header;
        QDataStream out(&header, QIODevice::WriteOnly);
        out << quint32(0x564F4943) // Magic 'VOIC'
            << t->transferId
            << t->fileName
            << t->totalBytes
            << t->sha256Hex;

        t->socket->write(header);
        emit transferStarted(t->transferId, t->fileName, t->totalBytes, true);
        sendNextChunk(*t);
    });

    connect(socket, &QTcpSocket::bytesWritten, this, [this, transferId](qint64) {
        if (!m_transfers.contains(transferId)) return;
        ActiveTransfer* t = m_transfers.value(transferId);
        if (t->direction == ActiveTransfer::Send && t->status == ActiveTransfer::InProgress) {
            sendNextChunk(*t);
        }
    });

    connect(socket, &QTcpSocket::disconnected, this, &SecureFileTransferManager::onSocketDisconnected);

    socket->connectToHost(targetPeer.ipAddress, targetPeer.filePort > 0 ? targetPeer.filePort : FILE_TRANSFER_PORT);
    return transferId;
}

void SecureFileTransferManager::sendNextChunk(ActiveTransfer& transfer) {
    if (!transfer.socket || !transfer.file) return;

    if (transfer.socket->bytesToWrite() > 128 * 1024) {
        return; // Wait for socket buffer drain
    }

    if (transfer.file->atEnd()) {
        if (transfer.transferredBytes >= transfer.totalBytes) {
            transfer.status = ActiveTransfer::Completed;
            emit transferCompleted(transfer.transferId, transfer.fileName, true);
            transfer.socket->disconnectFromHost();
        }
        return;
    }

    QByteArray chunk = transfer.file->read(FILE_CHUNK_SIZE);
    if (!chunk.isEmpty()) {
        transfer.socket->write(chunk);
        transfer.transferredBytes += chunk.size();

        double elapsedSec = transfer.timer.elapsed() / 1000.0;
        double mbps = (elapsedSec > 0.05) ? (transfer.transferredBytes / (1024.0 * 1024.0)) / elapsedSec : 0.0;
        emit transferProgress(transfer.transferId, transfer.transferredBytes, transfer.totalBytes, mbps);
    }
}

void SecureFileTransferManager::onNewServerConnection() {
    while (m_tcpServer && m_tcpServer->hasPendingConnections()) {
        QTcpSocket* socket = m_tcpServer->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &SecureFileTransferManager::onSocketReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &SecureFileTransferManager::onSocketDisconnected);
    }
}

void SecureFileTransferManager::onSocketReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (!m_socketToTransferId.contains(socket)) {
        // First packet on this socket: Read metadata header
        if (socket->bytesAvailable() < 24) return;

        QDataStream in(socket);
        quint32 magic = 0;
        in >> magic;
        if (magic != 0x564F4943) {
            qWarning() << "Invalid file transfer magic header received:" << Qt::hex << magic;
            socket->disconnectFromHost();
            return;
        }

        QString transferId, fileName, sha256Hex;
        qint64 totalBytes = 0;
        in >> transferId >> fileName >> totalBytes >> sha256Hex;

        ActiveTransfer* transfer = new ActiveTransfer();
        transfer->transferId = transferId;
        transfer->direction = ActiveTransfer::Receive;
        transfer->fileName = fileName;
        transfer->totalBytes = totalBytes;
        transfer->transferredBytes = 0;
        transfer->sha256Hex = sha256Hex;
        transfer->peerIp = socket->peerAddress().toString();
        transfer->localFilePath = m_downloadDir + "/" + fileName;
        transfer->status = ActiveTransfer::InProgress;
        transfer->socket = socket;
        transfer->timer.start();

        transfer->file = new QFile(transfer->localFilePath);
        if (!transfer->file->open(QIODevice::WriteOnly)) {
            emit transferFailed(transferId, "Could not open destination file for writing: " + transfer->localFilePath);
            delete transfer->file;
            delete transfer;
            socket->disconnectFromHost();
            return;
        }

        m_transfers.insert(transferId, transfer);
        m_socketToTransferId.insert(socket, transferId);
        emit transferStarted(transferId, fileName, totalBytes, false);
    }

    QString transferId = m_socketToTransferId.value(socket);
    if (!m_transfers.contains(transferId)) return;

    ActiveTransfer* t = m_transfers.value(transferId);
    if (t->direction != ActiveTransfer::Receive || !t->file) return;

    QByteArray data = socket->readAll();
    if (!data.isEmpty()) {
        t->file->write(data);
        t->transferredBytes += data.size();

        double elapsedSec = t->timer.elapsed() / 1000.0;
        double mbps = (elapsedSec > 0.05) ? (t->transferredBytes / (1024.0 * 1024.0)) / elapsedSec : 0.0;
        emit transferProgress(t->transferId, t->transferredBytes, t->totalBytes, mbps);

        if (t->transferredBytes >= t->totalBytes) {
            finishReceive(*t);
        }
    }
}

void SecureFileTransferManager::finishReceive(ActiveTransfer& transfer) {
    if (transfer.file) {
        transfer.file->flush();
        transfer.file->close();
    }

    // Verify SHA-256
    QString calculatedSha = CryptoEngine::computeFileSha256(transfer.localFilePath);
    bool verified = (calculatedSha.compare(transfer.sha256Hex, Qt::CaseInsensitive) == 0);

    transfer.status = ActiveTransfer::Completed;
    emit transferCompleted(transfer.transferId, transfer.fileName, verified);

    if (transfer.socket) {
        transfer.socket->disconnectFromHost();
    }
}

void SecureFileTransferManager::onSocketDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (m_socketToTransferId.contains(socket)) {
        QString transferId = m_socketToTransferId.value(socket);
        m_socketToTransferId.remove(socket);

        if (m_transfers.contains(transferId)) {
            ActiveTransfer* t = m_transfers.value(transferId);
            if (t->file) {
                t->file->close();
            }
            if (t->status == ActiveTransfer::InProgress) {
                t->status = ActiveTransfer::Failed;
                emit transferFailed(transferId, "Connection dropped during transfer");
            }
        }
    }
    socket->deleteLater();
}

bool SecureFileTransferManager::acceptTransfer(const QString& transferId) {
    return m_transfers.contains(transferId);
}

void SecureFileTransferManager::declineTransfer(const QString& transferId) {
    if (m_transfers.contains(transferId)) {
        ActiveTransfer* t = m_transfers.take(transferId);
        if (t->socket) t->socket->disconnectFromHost();
        if (t->file) {
            t->file->close();
            delete t->file;
        }
        delete t;
    }
}

QString SecureFileTransferManager::sendFile(const QString& targetIp, quint16 port, const QString& filePath) {
    PeerInfo peer;
    peer.ipAddress = targetIp;
    peer.username = targetIp;
    peer.filePort = port;
    return sendFile(filePath, peer);
}

} // namespace Onyx

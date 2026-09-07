#ifndef SECURE_FILE_TRANSFER_MANAGER_H
#define SECURE_FILE_TRANSFER_MANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QElapsedTimer>
#include "CryptoEngine.h"
#include "../core/JsonProtocol.h"
#include "../core/NetworkConfig.h"

namespace Onyx {

struct ActiveTransfer {
    enum Direction { Send, Receive };
    enum Status { Pending, InProgress, Completed, Failed };

    QString transferId;
    Direction direction;
    QString peerId;
    QString peerName;
    QString peerIp;
    QString localFilePath;
    QString fileName;
    qint64 totalBytes = 0;
    qint64 transferredBytes = 0;
    QString sha256Hex;
    QByteArray aesKey;
    QByteArray aesIv;
    Status status = Pending;
    QFile* file = nullptr;
    QTcpSocket* socket = nullptr;
    QElapsedTimer timer;
};

class SecureFileTransferManager : public QObject {
    Q_OBJECT
public:
    explicit SecureFileTransferManager(QObject* parent = nullptr);
    explicit SecureFileTransferManager(const QString& downloadDir, QObject* parent = nullptr);
    ~SecureFileTransferManager() override;

    bool startServer(quint16 port = FILE_TRANSFER_PORT);
    void stopServer();

    // Initiates sending a file to a peer
    QString sendFile(const QString& filePath, const PeerInfo& targetPeer);
    QString sendFile(const QString& targetIp, quint16 port, const QString& filePath);

    void setSecretKey(const QByteArray& key) { m_secretKey = key; }
    QByteArray secretKey() const { return m_secretKey; }

    // Accept or decline an incoming file transfer offer
    bool acceptTransfer(const QString& transferId);
    void declineTransfer(const QString& transferId);

    void setDownloadDirectory(const QString& path);
    QString downloadDirectory() const { return m_downloadDir; }

signals:
    void fileOfferReceived(const FileTransferOffer& offer);
    void transferStarted(const QString& transferId, const QString& fileName, qint64 totalBytes, bool isSending);
    void transferProgress(const QString& transferId, qint64 bytesTransferred, qint64 totalBytes, double speedMBs);
    void transferCompleted(const QString& transferId, const QString& fileName, bool verifiedSha256);
    void transferFailed(const QString& transferId, const QString& reason);

private slots:
    void onNewServerConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();

private:
    void sendNextChunk(ActiveTransfer& transfer);
    void finishReceive(ActiveTransfer& transfer);

    QTcpServer* m_tcpServer = nullptr;
    quint16 m_serverPort = FILE_TRANSFER_PORT;
    QString m_downloadDir;
    QByteArray m_secretKey;

    QMap<QString, ActiveTransfer*> m_transfers;
    QMap<QTcpSocket*, QString> m_socketToTransferId;
};

} // namespace Onyx

#endif // SECURE_FILE_TRANSFER_MANAGER_H

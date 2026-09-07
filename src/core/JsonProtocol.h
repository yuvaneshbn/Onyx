#ifndef JSON_PROTOCOL_H
#define JSON_PROTOCOL_H

#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <vector>
#include <json/json.h>

namespace Onyx {

struct PeerInfo {
    QString peerId;
    QString username;
    QString hostname;
    QString ipAddress;
    quint16 sipPort = 5060;
    quint16 filePort = 45455;
    quint16 chatPort = 45456;
    quint16 groupAudioPort = 45457;
    quint16 groupVideoPort = 45458;
    QString status = "online";
    QString statusMessage = "Available";
    QString avatarColor = "#0078D4";
    bool inCall = false;
    bool micMuted = false;
    bool videoMuted = false;
    bool isSpeaking = false;
    QString audioCodec = "opus";
    QString videoCodec = "vp8";
    qint64 lastSeenMs = 0;
};

struct ChatMessage {
    QString messageId;
    QString senderId;
    QString senderName;
    QString content;
    QString attachmentName;
    qint64 attachmentSize = 0;
    QString attachmentType; // "pdf", "image", "video", "file"
    qint64 timestamp = 0;
    bool isEncrypted = false;
};

struct CallSignal {
    enum Type {
        Invite,
        Ringing,
        Accept,
        Decline,
        Hangup,
        GroupJoin,
        GroupLeave,
        SpeakingState,
        MediaStateChange
    };

    Type type = Invite;
    QString callerId;
    QString callerName;
    QString targetId; // Empty for group multicast
    QString groupId = "general";
    bool videoEnabled = true;
    bool audioEnabled = true;
    bool isSpeaking = false;
    QString audioCodec = "opus";
    QString videoCodec = "vp8";
    quint16 audioPort = 45457;
    quint16 videoPort = 45458;
};

struct FileTransferOffer {
    QString transferId;
    QString senderId;
    QString senderName;
    QString fileName;
    qint64 fileSize = 0;
    QString sha256Hex;
    quint16 tcpPort = 45455;
    bool accepted = false;
};

class JsonProtocol {
public:
    // Discovery serialization
    static QByteArray createDiscoveryBeacon(const PeerInfo& localPeer);
    static bool parseDiscoveryBeacon(const QByteArray& data, PeerInfo& outPeer);

    // Chat serialization
    static QByteArray createChatMessage(const ChatMessage& msg);
    static bool parseChatMessage(const QByteArray& data, ChatMessage& outMsg);

    // Call signaling serialization
    static QByteArray createCallSignal(const CallSignal& signal);
    static bool parseCallSignal(const QByteArray& data, CallSignal& outSignal);

    // File transfer signaling serialization
    static QByteArray createFileOffer(const FileTransferOffer& offer);
    static bool parseFileOffer(const QByteArray& data, FileTransferOffer& outOffer);

    static QByteArray createFileResponse(const QString& transferId, bool accept);
    static bool parseFileResponse(const QByteArray& data, QString& outTransferId, bool& outAccepted);
};

} // namespace Onyx

#endif // JSON_PROTOCOL_H

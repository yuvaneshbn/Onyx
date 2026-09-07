#include "JsonProtocol.h"
#include <sstream>
#include <memory>

namespace Onyx {

static QByteArray jsonToString(const Json::Value& root) {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // Compact JSON
    std::string out = Json::writeString(builder, root);
    return QByteArray::fromStdString(out);
}

static bool parseJsonString(const QByteArray& data, Json::Value& root) {
    Json::CharReaderBuilder builder;
    std::string errs;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    const char* start = data.constData();
    return reader->parse(start, start + data.size(), &root, &errs);
}

QByteArray JsonProtocol::createDiscoveryBeacon(const PeerInfo& localPeer) {
    Json::Value root;
    root["type"] = "discovery";
    root["peerId"] = localPeer.peerId.toStdString();
    root["username"] = localPeer.username.toStdString();
    root["hostname"] = localPeer.hostname.toStdString();
    root["ipAddress"] = localPeer.ipAddress.toStdString();
    root["sipPort"] = localPeer.sipPort;
    root["filePort"] = localPeer.filePort;
    root["chatPort"] = localPeer.chatPort;
    root["groupAudioPort"] = localPeer.groupAudioPort;
    root["groupVideoPort"] = localPeer.groupVideoPort;
    root["status"] = localPeer.status.toStdString();
    root["statusMessage"] = localPeer.statusMessage.toStdString();
    root["avatarColor"] = localPeer.avatarColor.toStdString();
    root["inCall"] = localPeer.inCall;
    root["micMuted"] = localPeer.micMuted;
    root["videoMuted"] = localPeer.videoMuted;
    root["isSpeaking"] = localPeer.isSpeaking;
    root["audioCodec"] = localPeer.audioCodec.toStdString();
    root["videoCodec"] = localPeer.videoCodec.toStdString();
    root["timestamp"] = static_cast<Json::Int64>(QDateTime::currentMSecsSinceEpoch());
    return jsonToString(root);
}

bool JsonProtocol::parseDiscoveryBeacon(const QByteArray& data, PeerInfo& outPeer) {
    Json::Value root;
    if (!parseJsonString(data, root)) return false;
    if (!root.isMember("type") || root["type"].asString() != "discovery") return false;

    outPeer.peerId = QString::fromStdString(root.get("peerId", "").asString());
    outPeer.username = QString::fromStdString(root.get("username", "Unknown").asString());
    outPeer.hostname = QString::fromStdString(root.get("hostname", "UnknownHost").asString());
    outPeer.ipAddress = QString::fromStdString(root.get("ipAddress", "").asString());
    outPeer.sipPort = static_cast<quint16>(root.get("sipPort", 5060).asUInt());
    outPeer.filePort = static_cast<quint16>(root.get("filePort", 45455).asUInt());
    outPeer.chatPort = static_cast<quint16>(root.get("chatPort", 45456).asUInt());
    outPeer.groupAudioPort = static_cast<quint16>(root.get("groupAudioPort", 45457).asUInt());
    outPeer.groupVideoPort = static_cast<quint16>(root.get("groupVideoPort", 45458).asUInt());
    outPeer.status = QString::fromStdString(root.get("status", "online").asString());
    outPeer.statusMessage = QString::fromStdString(root.get("statusMessage", "Available").asString());
    outPeer.avatarColor = QString::fromStdString(root.get("avatarColor", "#0078D4").asString());
    outPeer.inCall = root.get("inCall", false).asBool();
    outPeer.micMuted = root.get("micMuted", false).asBool();
    outPeer.videoMuted = root.get("videoMuted", false).asBool();
    outPeer.isSpeaking = root.get("isSpeaking", false).asBool();
    outPeer.audioCodec = QString::fromStdString(root.get("audioCodec", "opus").asString());
    outPeer.videoCodec = QString::fromStdString(root.get("videoCodec", "vp8").asString());
    outPeer.lastSeenMs = QDateTime::currentMSecsSinceEpoch();
    return true;
}

QByteArray JsonProtocol::createChatMessage(const ChatMessage& msg) {
    Json::Value root;
    root["type"] = "chat";
    root["messageId"] = msg.messageId.toStdString();
    root["senderId"] = msg.senderId.toStdString();
    root["senderName"] = msg.senderName.toStdString();
    root["content"] = msg.content.toStdString();
    root["attachmentName"] = msg.attachmentName.toStdString();
    root["attachmentSize"] = static_cast<Json::Int64>(msg.attachmentSize);
    root["attachmentType"] = msg.attachmentType.toStdString();
    root["isEncrypted"] = msg.isEncrypted;
    root["timestamp"] = static_cast<Json::Int64>(msg.timestamp ? msg.timestamp : QDateTime::currentMSecsSinceEpoch());
    return jsonToString(root);
}

bool JsonProtocol::parseChatMessage(const QByteArray& data, ChatMessage& outMsg) {
    Json::Value root;
    if (!parseJsonString(data, root)) return false;
    if (!root.isMember("type") || root["type"].asString() != "chat") return false;

    outMsg.messageId = QString::fromStdString(root.get("messageId", "").asString());
    outMsg.senderId = QString::fromStdString(root.get("senderId", "").asString());
    outMsg.senderName = QString::fromStdString(root.get("senderName", "").asString());
    outMsg.content = QString::fromStdString(root.get("content", "").asString());
    outMsg.attachmentName = QString::fromStdString(root.get("attachmentName", "").asString());
    outMsg.attachmentSize = root.get("attachmentSize", 0).asInt64();
    outMsg.attachmentType = QString::fromStdString(root.get("attachmentType", "").asString());
    outMsg.isEncrypted = root.get("isEncrypted", false).asBool();
    outMsg.timestamp = root.get("timestamp", 0).asInt64();
    return true;
}

QByteArray JsonProtocol::createCallSignal(const CallSignal& signal) {
    Json::Value root;
    root["type"] = "call_signal";
    std::string sigType = "invite";
    switch (signal.type) {
        case CallSignal::Invite: sigType = "invite"; break;
        case CallSignal::Ringing: sigType = "ringing"; break;
        case CallSignal::Accept: sigType = "accept"; break;
        case CallSignal::Decline: sigType = "decline"; break;
        case CallSignal::Hangup: sigType = "hangup"; break;
        case CallSignal::GroupJoin: sigType = "group_join"; break;
        case CallSignal::GroupLeave: sigType = "group_leave"; break;
        case CallSignal::SpeakingState: sigType = "speaking_state"; break;
        case CallSignal::MediaStateChange: sigType = "media_state_change"; break;
    }
    root["signal"] = sigType;
    root["callerId"] = signal.callerId.toStdString();
    root["callerName"] = signal.callerName.toStdString();
    root["targetId"] = signal.targetId.toStdString();
    root["groupId"] = signal.groupId.toStdString();
    root["videoEnabled"] = signal.videoEnabled;
    root["audioEnabled"] = signal.audioEnabled;
    root["isSpeaking"] = signal.isSpeaking;
    root["audioCodec"] = signal.audioCodec.toStdString();
    root["videoCodec"] = signal.videoCodec.toStdString();
    root["audioPort"] = signal.audioPort;
    root["videoPort"] = signal.videoPort;
    return jsonToString(root);
}

bool JsonProtocol::parseCallSignal(const QByteArray& data, CallSignal& outSignal) {
    Json::Value root;
    if (!parseJsonString(data, root)) return false;
    if (!root.isMember("type") || root["type"].asString() != "call_signal") return false;

    std::string sigType = root.get("signal", "").asString();
    if (sigType == "invite") outSignal.type = CallSignal::Invite;
    else if (sigType == "ringing") outSignal.type = CallSignal::Ringing;
    else if (sigType == "accept") outSignal.type = CallSignal::Accept;
    else if (sigType == "decline") outSignal.type = CallSignal::Decline;
    else if (sigType == "hangup") outSignal.type = CallSignal::Hangup;
    else if (sigType == "group_join") outSignal.type = CallSignal::GroupJoin;
    else if (sigType == "group_leave") outSignal.type = CallSignal::GroupLeave;
    else if (sigType == "speaking_state") outSignal.type = CallSignal::SpeakingState;
    else if (sigType == "media_state_change") outSignal.type = CallSignal::MediaStateChange;
    else return false;

    outSignal.callerId = QString::fromStdString(root.get("callerId", "").asString());
    outSignal.callerName = QString::fromStdString(root.get("callerName", "").asString());
    outSignal.targetId = QString::fromStdString(root.get("targetId", "").asString());
    outSignal.groupId = QString::fromStdString(root.get("groupId", "general").asString());
    outSignal.videoEnabled = root.get("videoEnabled", true).asBool();
    outSignal.audioEnabled = root.get("audioEnabled", true).asBool();
    outSignal.isSpeaking = root.get("isSpeaking", false).asBool();
    outSignal.audioCodec = QString::fromStdString(root.get("audioCodec", "opus").asString());
    outSignal.videoCodec = QString::fromStdString(root.get("videoCodec", "vp8").asString());
    outSignal.audioPort = static_cast<quint16>(root.get("audioPort", 45457).asUInt());
    outSignal.videoPort = static_cast<quint16>(root.get("videoPort", 45458).asUInt());
    return true;
}

QByteArray JsonProtocol::createFileOffer(const FileTransferOffer& offer) {
    Json::Value root;
    root["type"] = "file_offer";
    root["transferId"] = offer.transferId.toStdString();
    root["senderId"] = offer.senderId.toStdString();
    root["senderName"] = offer.senderName.toStdString();
    root["fileName"] = offer.fileName.toStdString();
    root["fileSize"] = static_cast<Json::Int64>(offer.fileSize);
    root["sha256Hex"] = offer.sha256Hex.toStdString();
    root["tcpPort"] = offer.tcpPort;
    return jsonToString(root);
}

bool JsonProtocol::parseFileOffer(const QByteArray& data, FileTransferOffer& outOffer) {
    Json::Value root;
    if (!parseJsonString(data, root)) return false;
    if (!root.isMember("type") || root["type"].asString() != "file_offer") return false;

    outOffer.transferId = QString::fromStdString(root.get("transferId", "").asString());
    outOffer.senderId = QString::fromStdString(root.get("senderId", "").asString());
    outOffer.senderName = QString::fromStdString(root.get("senderName", "").asString());
    outOffer.fileName = QString::fromStdString(root.get("fileName", "").asString());
    outOffer.fileSize = root.get("fileSize", 0).asInt64();
    outOffer.sha256Hex = QString::fromStdString(root.get("sha256Hex", "").asString());
    outOffer.tcpPort = static_cast<quint16>(root.get("tcpPort", 45455).asUInt());
    return true;
}

QByteArray JsonProtocol::createFileResponse(const QString& transferId, bool accept) {
    Json::Value root;
    root["type"] = "file_response";
    root["transferId"] = transferId.toStdString();
    root["accepted"] = accept;
    return jsonToString(root);
}

bool JsonProtocol::parseFileResponse(const QByteArray& data, QString& outTransferId, bool& outAccepted) {
    Json::Value root;
    if (!parseJsonString(data, root)) return false;
    if (!root.isMember("type") || root["type"].asString() != "file_response") return false;

    outTransferId = QString::fromStdString(root.get("transferId", "").asString());
    outAccepted = root.get("accepted", false).asBool();
    return true;
}

} // namespace Onyx

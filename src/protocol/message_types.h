#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace protocol {

enum class MessageType : uint8_t {
    // Registration & Setup
    DISCOVERY = 0x01,
    STUN_BINDING_REQUEST = 0x02,
    STUN_BINDING_RESPONSE = 0x03,

    // Tree Maintenance
    HEARTBEAT = 0x10,
    JOIN_TREE = 0x11,
    JOIN_ACK = 0x12,
    ROUTER_ELECTION = 0x13,
    FAILOVER_TRIGGER = 0x14,

    // Audio Transport
    RTP = 0x20,
    RTCP = 0x21,
    
    // Auth & Control
    AUTH_CHALLENGE = 0x30,
    AUTH_RESPONSE = 0x31

};

// Basic structures to be built upon by Serializer

struct RtpPacket {
    uint8_t version;
    bool marker;
    uint8_t payload_type;
    uint16_t sequence_number;
    uint32_t timestamp;
    uint32_t ssrc;
    std::vector<uint8_t> payload; // Opus frame
};

struct Heartbeat {
    uint64_t client_id;
    bool is_router;
    uint32_t active_children_count;
    uint32_t cpu_load;
};

} // namespace protocol

// Q_DECLARE_METATYPE for Qt signal/slot queuing
#include <QMetaType>
Q_DECLARE_METATYPE(protocol::RtpPacket)

#endif // MESSAGE_TYPES_H

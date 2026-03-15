#include "serializer.h"

namespace protocol {

void Serializer::writeU16(std::vector<uint8_t>& buf, uint16_t val) {
    uint16_t netVal = htons(val);
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&netVal);
    buf.insert(buf.end(), ptr, ptr + sizeof(uint16_t));
}

void Serializer::writeU32(std::vector<uint8_t>& buf, uint32_t val) {
    uint32_t netVal = htonl(val);
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&netVal);
    buf.insert(buf.end(), ptr, ptr + sizeof(uint32_t));
}

void Serializer::writeU64(std::vector<uint8_t>& buf, uint64_t val) {
    // htonl only handles 32 bit, we need to handle 64 bit manually or with htonll if available.
    // Manual bit shift for portability:
    buf.push_back((val >> 56) & 0xFF);
    buf.push_back((val >> 48) & 0xFF);
    buf.push_back((val >> 40) & 0xFF);
    buf.push_back((val >> 32) & 0xFF);
    buf.push_back((val >> 24) & 0xFF);
    buf.push_back((val >> 16) & 0xFF);
    buf.push_back((val >> 8) & 0xFF);
    buf.push_back(val & 0xFF);
}

uint16_t Serializer::readU16(const uint8_t*& ptr, size_t& remaining) {
    if (remaining < sizeof(uint16_t)) throw std::runtime_error("Buffer underflow U16");
    uint16_t val;
    std::memcpy(&val, ptr, sizeof(uint16_t));
    ptr += sizeof(uint16_t);
    remaining -= sizeof(uint16_t);
    return ntohs(val);
}

uint32_t Serializer::readU32(const uint8_t*& ptr, size_t& remaining) {
    if (remaining < sizeof(uint32_t)) throw std::runtime_error("Buffer underflow U32");
    uint32_t val;
    std::memcpy(&val, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    remaining -= sizeof(uint32_t);
    return ntohl(val);
}

uint64_t Serializer::readU64(const uint8_t*& ptr, size_t& remaining) {
    if (remaining < sizeof(uint64_t)) throw std::runtime_error("Buffer underflow U64");
    uint64_t val = 
        (static_cast<uint64_t>(ptr[0]) << 56) |
        (static_cast<uint64_t>(ptr[1]) << 48) |
        (static_cast<uint64_t>(ptr[2]) << 40) |
        (static_cast<uint64_t>(ptr[3]) << 32) |
        (static_cast<uint64_t>(ptr[4]) << 24) |
        (static_cast<uint64_t>(ptr[5]) << 16) |
        (static_cast<uint64_t>(ptr[6]) << 8) |
        (static_cast<uint64_t>(ptr[7]));
    ptr += sizeof(uint64_t);
    remaining -= sizeof(uint64_t);
    return val;
}

std::vector<uint8_t> Serializer::serializeRtp(const RtpPacket& packet) {
    std::vector<uint8_t> buf;
    buf.reserve(12 + packet.payload.size());

    // Byte 0: Version (2 bits), Padding (1 bit), Extension (1 bit), CSRC Count (4 bits)
    // For simplicity, we assume Version=2, No padding, no extension, no CSRC
    uint8_t byte0 = (packet.version << 6) & 0xC0;
    buf.push_back(byte0);

    // Byte 1: Marker (1 bit), Payload Type (7 bits)
    uint8_t byte1 = ((packet.marker ? 1 : 0) << 7) | (packet.payload_type & 0x7F);
    buf.push_back(byte1);

    writeU16(buf, packet.sequence_number);
    writeU32(buf, packet.timestamp);
    writeU32(buf, packet.ssrc);

    // Append payload
    if (!packet.payload.empty()) {
        buf.insert(buf.end(), packet.payload.begin(), packet.payload.end());
    }

    return buf;
}

RtpPacket Serializer::deserializeRtp(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 12) throw std::runtime_error("Buffer too small for RTP header");

    RtpPacket packet;
    const uint8_t* ptr = buffer.data();
    size_t remaining = buffer.size();

    uint8_t byte0 = ptr[0];
    packet.version = (byte0 >> 6) & 0x03;
    
    uint8_t byte1 = ptr[1];
    packet.marker = (byte1 >> 7) & 0x01;
    packet.payload_type = byte1 & 0x7F;

    ptr += 2;
    remaining -= 2;

    packet.sequence_number = readU16(ptr, remaining);
    packet.timestamp = readU32(ptr, remaining);
    packet.ssrc = readU32(ptr, remaining);

    if (remaining > 0) {
        packet.payload.assign(ptr, ptr + remaining);
    }

    return packet;
}

std::vector<uint8_t> Serializer::serializeHeartbeat(const Heartbeat& hb) {
    std::vector<uint8_t> buf;
    buf.reserve(sizeof(uint64_t) + 1 + sizeof(uint32_t) + sizeof(uint32_t));
    
    writeU64(buf, hb.client_id);
    buf.push_back(hb.is_router ? 1 : 0);
    writeU32(buf, hb.active_children_count);
    writeU32(buf, hb.cpu_load);

    return buf;
}

Heartbeat Serializer::deserializeHeartbeat(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 17) throw std::runtime_error("Buffer too small for Heartbeat");

    Heartbeat hb;
    const uint8_t* ptr = buffer.data();
    size_t remaining = buffer.size();

    hb.client_id = readU64(ptr, remaining);
    hb.is_router = (ptr[0] != 0);
    ptr += 1;
    remaining -= 1;
    hb.active_children_count = readU32(ptr, remaining);
    hb.cpu_load = readU32(ptr, remaining);

    return hb;
}

} // namespace protocol

#ifndef SERIALIZER_H
#define SERIALIZER_H

#pragma once

#include "message_types.h"
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace protocol {

// A lightweight, custom binary serialization utility to pack/unpack struct data
// safely for UDP transmission. Uses network byte order (Big Endian).
class Serializer {
public:
    static std::vector<uint8_t> serializeRtp(const RtpPacket& packet);
    static RtpPacket deserializeRtp(const std::vector<uint8_t>& buffer);

    static std::vector<uint8_t> serializeHeartbeat(const Heartbeat& hb);
    static Heartbeat deserializeHeartbeat(const std::vector<uint8_t>& buffer);

private:
    static void writeU16(std::vector<uint8_t>& buf, uint16_t val);
    static void writeU32(std::vector<uint8_t>& buf, uint32_t val);
    static void writeU64(std::vector<uint8_t>& buf, uint64_t val);

    static uint16_t readU16(const uint8_t*& ptr, size_t& remaining);
    static uint32_t readU32(const uint8_t*& ptr, size_t& remaining);
    static uint64_t readU64(const uint8_t*& ptr, size_t& remaining);
};

} // namespace protocol

#endif // SERIALIZER_H

#ifndef JITTER_BUFFER_H
#define JITTER_BUFFER_H

#pragma once

#include <vector>
#include <map>
#include <mutex>

namespace utils {

// Fixed-size Jitter Buffer for RTP audio frames.
// Reorders packets based on sequence number and timestamp.
// Handles late packets (dropped if too late) and missing packets (yields empty for PLC).
class JitterBuffer {
public:
    explicit JitterBuffer(int capacityFrames = 10);

    // Push an incoming RTP payload into the buffer.
    void push(uint16_t seqNum, const std::vector<uint8_t>& payload);

    // Pop the next expected sequence number.
    // Returns empty vector if the packet hasn't arrived (lost/delayed).
    // Automatically advances the read pointer.
    std::vector<uint8_t> pop();

    // Forces the buffer to reset if the sequence number jumps significantly
    void reset();

protected:
    int capacity_;
    std::map<uint16_t, std::vector<uint8_t>> buffer_;
    std::mutex mutex_;
    
    uint16_t nextReadSeq_{0};
    bool isFirstPacket_{true};
};

} // namespace utils

#endif // JITTER_BUFFER_H

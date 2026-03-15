#include "jitter_buffer.h"

namespace utils {

JitterBuffer::JitterBuffer(int capacityFrames) : capacity_(capacityFrames) {}

void JitterBuffer::push(uint16_t seqNum, const std::vector<uint8_t>& payload) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (isFirstPacket_) {
        nextReadSeq_ = seqNum;
        isFirstPacket_ = false;
    }

    // Is it too late? (A simplistic check handling wrap-around)
    int16_t diff = seqNum - nextReadSeq_;
    if (diff < 0) {
        return; // Packet arrived after we already played that slot
    }

    // Is it too far ahead?
    if (diff >= capacity_) {
        // Fast-forward the buffer (we missed a huge chunk or stalled)
        nextReadSeq_ = static_cast<uint16_t>(seqNum - capacity_ / 2); 
        
        // Clear out old packets
        auto it = buffer_.begin();
        while (it != buffer_.end()) {
            if (static_cast<int16_t>(it->first - nextReadSeq_) < 0) {
                it = buffer_.erase(it);
            } else {
                ++it;
            }
        }
    }

    buffer_[seqNum] = payload;
}

std::vector<uint8_t> JitterBuffer::pop() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (isFirstPacket_) {
        return {}; // Nothing to play yet
    }

    std::vector<uint8_t> result;
    auto it = buffer_.find(nextReadSeq_);

    if (it != buffer_.end()) {
        result = it->second;
        buffer_.erase(it);
    } 
    // Else: result remains empty, signaling the decoder to run PLC (Packet Loss Concealment)

    nextReadSeq_++;
    return result;
}

void JitterBuffer::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    buffer_.clear();
    isFirstPacket_ = true;
}

} // namespace utils

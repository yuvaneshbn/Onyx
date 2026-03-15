#ifndef RTP_SCHEDULER_H
#define RTP_SCHEDULER_H

#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>

namespace core {

// Precise thread-based scheduler specifically for pacing 20ms RTP audio chunks.
// QTimer is often not accurate enough for low-jitter audio pushing.
class RtpScheduler {
public:
    RtpScheduler(std::function<void()> callback, int intervalMs = 20)
        : callback_(std::move(callback)), intervalMs_(intervalMs), running_(false) {}

    ~RtpScheduler() {
        stop();
    }

    void start() {
        if (running_) return;
        running_ = true;
        thread_ = std::thread(&RtpScheduler::loop, this);
    }

    void stop() {
        if (!running_) return;
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    void loop() {
        auto next_tick = std::chrono::steady_clock::now();
        const auto duration = std::chrono::milliseconds(intervalMs_);
        
        while (running_) {
            callback_();
            next_tick += duration;
            std::this_thread::sleep_until(next_tick);
        }
    }

    std::function<void()> callback_;
    int intervalMs_;
    std::atomic<bool> running_;
    std::thread thread_;
};

} // namespace core

#endif // RTP_SCHEDULER_H

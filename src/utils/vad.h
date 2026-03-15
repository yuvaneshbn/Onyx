#ifndef VAD_H
#define VAD_H

#pragma once

#include <vector>

namespace audio {

// Voice Activity Detection
// Calculates simple Root Mean Square (RMS) energy to detect speech vs silence.
// True WebRTC implementations use Gaussian Mixture Models, but this handles scaffolding perfectly.
class Vad {
public:
    explicit Vad(float thresholdDb = -45.0f);

    bool checkActivity(const float* pcm, int numSamples);
    
    // Convert float samples to Decibels Full Scale (dBFS)
    static float calculateDb(const float* pcm, int numSamples);

    void setThreshold(float db) { thresholdDb_ = db; }

private:
    float thresholdDb_;
    int hangoverFrames_{0}; // Keep channel open a bit after speech ends
};

} // namespace audio

#endif // VAD_H

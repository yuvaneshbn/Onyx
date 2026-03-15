#include "vad.h"
#include <cmath>

namespace audio {

Vad::Vad(float thresholdDb) : thresholdDb_(thresholdDb) {}

float Vad::calculateDb(const float* pcm, int numSamples) {
    if (numSamples == 0 || pcm == nullptr) return -100.0f; // Silence

    float sumSquares = 0.0f;
    for (int i = 0; i < numSamples; ++i) {
        sumSquares += pcm[i] * pcm[i];
    }
    
    float rms = std::sqrt(sumSquares / numSamples);
    
    if (rms < 1e-5f) return -100.0f; // Prevent log(0)
    
    return 20.0f * std::log10(rms);
}

bool Vad::checkActivity(const float* pcm, int numSamples) {
    float db = calculateDb(pcm, numSamples);
    
    if (db > thresholdDb_) {
        hangoverFrames_ = 50; // Typically 1 second at 50 frames/sec
        return true;
    } 
    
    if (hangoverFrames_ > 0) {
        hangoverFrames_--;
        return true; // Still speaking but maybe taking a quick breath
    }
    
    return false; // Silence
}

} // namespace audio

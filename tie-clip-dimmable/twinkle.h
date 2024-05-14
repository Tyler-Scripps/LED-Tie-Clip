#ifndef TWINKLE_H
#define TWINKLE_H
#include "modeHelpers.h"
#include "modeBase.h"

class Twinkle : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
    private:
        float stepSize = 0.1;
        uint8_t currentLed;
        bool brightening = true;
        float currentBrightness = 0;
};


void Twinkle::calculateNextFrame(uint32_t* arrPtr) {
    // zero out led array
    for (uint8_t i = 0; i < numLeds; i++) {
        arrPtr[i] = 0;
    }

    if (brightening) {
        currentBrightness += stepSize;
        if (currentBrightness > 32*brightness) {
            brightening = false;
        }
    } else {
        currentBrightness -= stepSize;
        if (currentBrightness < 0) {
            brightening = true;
            currentLed = random(0, numLeds);
        }
    }

    arrPtr[currentLed] = brightnessToUint32(currentBrightness);
}

#endif
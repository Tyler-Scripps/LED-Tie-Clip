#ifndef TWINKLE_H
#include "modeHelpers.h"
#include "modeBase.h"

class Twinkle : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
        void init(uint8_t newLedNum);
    private:
        // float* As;  // magnitude
        // float* Bs;  // frequency
        // float* Cs;  // phase shift
        // uint8_t* Ds;  // vertical offset
        // float currentPos = 0;
        float stepSize = 0.1;
        uint8_t currentLed;
        bool brightening = true;
        float currentBrightness = 0;
};

void Twinkle::init(uint8_t newLedNum) {
    numLeds = newLedNum;
    currentLed = random(0, numLeds);
    // As = new float[numLeds];
    // Bs = new float[numLeds];
    // Cs = new float[numLeds];
    // Ds = new uint8_t[numLeds];
    // for (uint8_t i = 0; i < numLeds; i++) {
    //     // As[i] = random(1, 320) / 5.0;
    //     As[i] = 96;
    //     Bs[i] = random(1, 2000) / 1000.0;
    //     Cs[i] = random(0, 100) / 10.0;
    //     Ds[i] = random(64, 95);
    // }
}

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
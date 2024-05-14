#ifndef SIDE_SCROLL_H
#define SIDE_SCROLL_H
#include "modeHelpers.h"
#include "modeBase.h"

class SideScroll : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
    private:
        uint8_t position;
        uint8_t speed = 30;
        uint8_t currentIteration = 0;
};

void SideScroll::calculateNextFrame(uint32_t* arrPtr) {
    for (uint8_t i = 0; i < numLeds; i++) {
        arrPtr[i] = 0;
    }

    uint32_t tempBrightness = brightnessToUint32(brightness * 32);
    // position++;
    if (position > 7) {
        position = 0;
    }
    for (uint8_t i = 0; i < numLeds; i++)
    {
        if (i % 8 == position) {
            arrPtr[i] = tempBrightness;
        }
    }
    currentIteration++;
    if (currentIteration >= speed) {
        currentIteration = 0;
        position++;
    }
}

#endif // SIDE_SCROLL_H
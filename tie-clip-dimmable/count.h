#ifndef COUNT_H
#define COUNT_H
#include "modeHelpers.h"
#include "modeBase.h"

class Count : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
    private:
        uint32_t currentCount = 0;
};

void Count::calculateNextFrame(uint32_t* arrPtr) {
    uint8_t countLength = (numLeds < 32) ? numLeds : 32;
    uint32_t tempBrightness = brightnessToUint32(brightness * 32);
    for (uint8_t i = 0; i < countLength; i++)
    {
        arrPtr[i] = tempBrightness * ((currentCount >> (31-i)) & 1);
    }
    currentCount++;
}

#endif
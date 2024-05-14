#ifndef ALTERNATE_H
#define ALTERNATE_H

class Alternate : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
    private:
        bool currentState = 0;
        uint8_t speed = 30;
        uint8_t currentIteration = 0;
};

void Alternate::calculateNextFrame(uint32_t* arrPtr) {
    for (uint8_t i = 0; i < numLeds; i++) {
        arrPtr[i] = 0;
    }

    uint32_t tempBrightness = brightnessToUint32(brightness * 32.0);
    for (uint8_t i = 0; i <  numLeds; i++) {
        if ((i+currentState)%2) {
            arrPtr[i] = tempBrightness;
        } else {
            arrPtr[i] = 0;
        }
    }
    currentIteration++;
    if (currentIteration >= speed) {
        currentIteration = 0;
        currentState = !currentState;
    }
}

#endif
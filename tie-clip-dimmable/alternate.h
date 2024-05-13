#ifndef ALTERNATE_H
#define ALTERNATE_H

class Alternate : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
    private:
        bool currentState = 0;
};

void Alternate::calculateNextFrame(uint32_t* arrPtr) {
    uint32_t tempBrightness = brightnessToUint32(brightness * 32);
    for (uint8_t i = 0; i <  numLeds; i++) {
        if (i%2) {
            arrPtr[i] = tempBrightness;
        } else {
            arrPtr[i] = 0;
        }
    }
    currentState = !currentState;
}

#endif
#ifndef MODE_BASE_H
#define MODE_BASE_H

class ModeBase {
    public:
        void init(uint8_t newLedNum);
        virtual void calculateNextFrame(uint32_t* arrPtr);
        void setBrightness(float newBrightness);
    protected:
        uint8_t numLeds;
        float brightness = 0.25;
};

void ModeBase::init(uint8_t newLedNum) {
    numLeds = newLedNum;

}

void ModeBase::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

#endif
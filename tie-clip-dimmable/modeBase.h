#ifndef MODE_BASE_H
#define MODE_BASE_H

class ModeBase {
    public:
        void init(uint8_t newLedNum);
        void init(uint8_t newLedNum, float brightness);
        virtual void calculateNextFrame(uint32_t* arrPtr);
        void setBrightness(float newBrightness);
    protected:
        uint8_t numLeds;
        float brightness = 0.25;
};

void ModeBase::init(uint8_t newLedNum) {
    numLeds = newLedNum;

}

void ModeBase::init(uint8_t newLedNum, float newBrightness) {
    this->numLeds = newLedNum;
    this->brightness = brightness;
}

void ModeBase::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

#endif
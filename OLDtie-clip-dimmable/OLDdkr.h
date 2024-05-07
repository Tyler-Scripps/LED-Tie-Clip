// #include <cstdint>
#ifndef DKR

void setNthBit(uint8_t* array, int index, bool value) {
    if (value) {
        array[index / 8] |= (1 << (index % 8));
    } else {
        array[index / 8] &= ~(1 << (index % 8));
    }
}

class DKR {
    public:
        // DKR();
        void init(uint8_t newLedNum, uint8_t neweArrSize);
        void calculateNextSubFrame(uint8_t* arrPtr);
        void calculateNextFrame();
        void setWidth(uint8_t newWidth);
    private:
        uint8_t numLeds;
        uint8_t arrSize;
        uint8_t currSubFrame = 0;
        uint8_t speed = 10;
        uint8_t position = 0;   // center of lit leds
        uint8_t width = 5;
        uint8_t* brightnesses;
        uint8_t* mappedBrightnesses;
        bool direction;
        uint8_t numSubFrames = 32;  // number of sub frames before going to next frame
};

// DKR::DKR(uint8_t newNum, uint8_t newSize) {
//     numLeds = newNum;
//     arrSize = newSize;
//     brightnesses = new uint8_t[numLeds];
//     mappedBrightnesses = new uint8_t[numLeds];
// }

void DKR::init(uint8_t newLedNum, uint8_t neweArrSize) {
    numLeds = newLedNum;
    arrSize = neweArrSize;
    brightnesses = new uint8_t[numLeds];
    mappedBrightnesses = new uint8_t[numLeds];
}

void DKR::calculateNextSubFrame(uint8_t* arrPtr) {
    for (uint8_t i = 0; i < numLeds; i++) {
        if (mappedBrightnesses[i] < currSubFrame) {
            setNthBit(arrPtr, i, 1);
        } else {
            setNthBit(arrPtr, i, 0);
        }
    }
    currSubFrame++;
    if (currSubFrame >= numSubFrames) {
        currSubFrame = 0;
        calculateNextFrame();
    }
}

void DKR::calculateNextFrame() {
    if (position == 0 || position == numLeds-1) {
        direction = !direction;
    }

    if (direction) {
        position++;
    } else {
        position--;
    }

    brightnesses[position] = 255;
    uint8_t numSteps = ((width-1)/2);
    for (uint8_t i = 0; i < numSteps; i++) {
        uint8_t brightnessStep = 255 - ((255/(numSteps+1))*i);
        int16_t upperIndex = position+i;
        int16_t lowerIndex = position-1;
        if (lowerIndex >= 0) {
            brightnesses[lowerIndex] = brightnessStep;
        }
        if (upperIndex < numLeds) {
            brightnesses[upperIndex] = brightnessStep;
        }
    }

    for (uint8_t i = 0; i < numLeds; i++) {
        mappedBrightnesses[i] = map(brightnesses[i], 0, 255, 0, numSubFrames);
    }
}

void DKR::setWidth(uint8_t newWidth) {
    if(width == 0) {
        newWidth = 1;
    }

    //id new width is even
    if (!(newWidth % 2)) {
        newWidth++;
    }
    width = newWidth;
}

#endif
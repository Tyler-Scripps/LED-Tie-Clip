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
        void init(uint8_t newLedNum);
        // void calculateNextSubFrame(uint8_t* arrPtr);
        void calculateNextFrame(uint16_t* arrPtr);
        void setWidth(uint8_t newWidth);
        uint16_t brightnessToUint16(uint8_t brightness);
    private:
        uint8_t numLeds;
        float speed = 0.15;   // number of leds to move per frame
        float centerPosition = 0;
        uint8_t width = 13;
        bool direction;
        float brightness = 0.25;
};


void DKR::init(uint8_t newLedNum) {
    numLeds = newLedNum;

}


void DKR::calculateNextFrame(uint16_t* arrPtr) {
    // if at the end flip the direction
    if (centerPosition <= 0 || centerPosition >= numLeds-1) {
        direction = !direction;
    }

    // move in current direction
    if (direction) {
        centerPosition += speed;
    } else {
        centerPosition -= speed;
    }

    // zero out led array
    for (uint8_t i = 0; i < numLeds; i++) {
        arrPtr[i] = 0;
    }

    uint8_t brightnessStep;
    for (uint8_t i = 0; i < numLeds; i++) {
        // if the led is too far away it must be zero so we can skip doing trig
        float diff = centerPosition - i;
        diff = abs(diff);
        if (diff < (width/2)) {
            // i takes the place of x
            brightnessStep = (uint8_t)((8*sin((TWO_PI/(width-1))*(i-centerPosition) + HALF_PI) + 8)*brightness);
            arrPtr[i] = brightnessToUint16(brightnessStep);
        }
    }

}

void DKR::setWidth(uint8_t newWidth) {
    if(width == 0) {
        newWidth = 1;
    }

    //if new width is even make is odd by adding 1
    if (!(newWidth % 2)) {
        newWidth++;
    }
    width = newWidth;
}

uint16_t DKR::brightnessToUint16(uint8_t brightness) {
    uint16_t calculatedValue;
    switch (brightness) {
        case 0:
            calculatedValue = 0;
            break;
        case 1:
            calculatedValue = 0b0000000000000001;
            break;
        case 2:
            calculatedValue = 0b0000000100000001;
            break;
        case 3:
            calculatedValue = 0b0000010000100001;
            break;
        case 4:
            calculatedValue = 0b0001000100010001;
            break;
        case 5:
            calculatedValue = 0b0001001001001001;
            break;
        case 6:
            calculatedValue = 0b0010010010010101;
            break;
        case 7:
            calculatedValue = 0b0001010101010101;
            break;
        case 8:
            calculatedValue = 0b0101010101010101;
            break;
        case 9:
            calculatedValue = 0b0101010101010111;
            break;
        case 10:
            calculatedValue = 0b0101010101011111;
            break;
        case 11:
            calculatedValue = 0b0101010101111111;
            break;
        case 12:
            calculatedValue = 0b0101010111111111;
            break;
        case 13:
            calculatedValue = 0b0101011111111111;
            break;
        case 14:
            calculatedValue = 0b0101111111111111;
            break;
        case 15:
            calculatedValue = 0b0111111111111111;
            break;
        case 16:
            calculatedValue = 0b1111111111111111;
            break;
        default:
            calculatedValue = 0b1111111111111111;
            break;
    }
    return calculatedValue;
}

#endif
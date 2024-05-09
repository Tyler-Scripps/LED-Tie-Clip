#ifndef DKR_H
#define DKR_H
#include "modeBase.h"
#include "modeHelpers.h"

class DKR : public ModeBase {
    public:
        void calculateNextFrame(uint32_t* arrPtr) override;
        void setWidth(uint8_t newWidth);
    private:
        float speed = 0.15;   // number of leds to move per frame
        float centerPosition = 0;
        uint8_t width = 11;
        bool direction;
};



void DKR::calculateNextFrame(uint32_t* arrPtr) {
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
            brightnessStep = (uint8_t)((16*sin((TWO_PI/(width-1))*(i-centerPosition) + HALF_PI) + 16)*brightness);
            arrPtr[i] = brightnessToUint32(brightnessStep);
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


#endif
// defines for attiny_timerinterrupt.h
// Select USING_HALF_CLOCK == true for  10/ 8MHz to Timer TCBx => shorter timer, but better accuracy
#define USING_HALF_CLOCK true

// use ITimer1 if ITimer0 is used by millis() Servo or tone()
#define USE_TIMER_0 true
#define CurrentTimer ITimer0

#include "ATtiny_TimerInterrupt.h"

#include "dkr.h"
#include "twinkle.h"
#include "count.h"
#include "alternate.h"
#include "sideScroll.h"

#define DATA_PIN 5
#define CLOCK_PIN 4
#define OE_PIN 6
#define LATCH_PIN 7
#define BUTTON1_PIN 0
#define BUTTON2_PIN 10

#define NUM_LEDS 32
#define NUM_MODES 5
#define BRIGHTNESS 0.125

uint32_t frameBufs[2][NUM_LEDS] = {0};

volatile uint8_t currentSubFrame = 0;

volatile bool currentFrameBuf = 0;
volatile bool doneCalculating = false;

uint8_t brightness = 255;

unsigned long lastButton1 = 0;
unsigned long lastButton2 = 0;

uint8_t mode = 0;
uint16_t speed = 125;
uint8_t speedBase = 1;

DKR dkr;
Twinkle twinkle;
Count count;
Alternate alternate;
SideScroll sidescroll;

void setup() {
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(OE_PIN, LOW);
    digitalWrite(LATCH_PIN, LOW);

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2, CHANGE);

    displayLeds();
    // analogWrite(OE_PIN, 255 - brightness);

    dkr.init(NUM_LEDS, BRIGHTNESS);
    twinkle.init(NUM_LEDS, BRIGHTNESS);
    count.init(NUM_LEDS, BRIGHTNESS);
    alternate.init(NUM_LEDS, BRIGHTNESS);
    sidescroll.init(NUM_LEDS, BRIGHTNESS);

    CurrentTimer.init();

    //may be running at half speed because attiny running at only 10mhz
    CurrentTimer.attachInterrupt(30*32*2, displayLeds);

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        frameBufs[0][i] = 0x0;
        frameBufs[1][i] = 0x0;
    }
}

void loop() {
    if (!doneCalculating) {
        switch (mode)
        {
        case 0:
            dkr.calculateNextFrame(frameBufs[!currentFrameBuf]);
            break;
        case 1:
            twinkle.calculateNextFrame(frameBufs[!currentFrameBuf]);
            break;
        case 2:
            count.calculateNextFrame(frameBufs[!currentFrameBuf]);
            break;
        case 3:
            alternate.calculateNextFrame(frameBufs[!currentFrameBuf]);
            break;
        case 4:
            sidescroll.calculateNextFrame(frameBufs[!currentFrameBuf]);
            break;
        default:
            break;
        }
        doneCalculating = true;
    }
}

void handleButton1() {
    if (millis() - lastButton1 < 500) {
        return;
    }
    lastButton1 = millis();
    fullOn(frameBufs[currentFrameBuf]);
    displayLeds();
    delay(1000);
    fullOff(frameBufs[currentFrameBuf]);
    displayLeds();
    cycleMode();
}

void handleButton2() {
    if (millis() - lastButton2 < 500) {
        return;
    }
    lastButton2 = millis();
    fullOn(frameBufs[currentFrameBuf]);
    displayLeds();
    delay(1000);
    fullOff(frameBufs[currentFrameBuf]);
    displayLeds();
    cycleSpeed();
}

// this should be run at 32 times the desired framerate of the animation
void displayLeds() {
    currentSubFrame++;

    if (currentSubFrame >= 32 && doneCalculating) {   // finsished frame and next is done calculating
        currentSubFrame = 0;
        currentFrameBuf = !currentFrameBuf;
        doneCalculating = false;
    } else if (currentSubFrame >= 32) {   // finished frame but next is still calculating
        currentSubFrame = 0;
    }

    uint16_t mask = 1 << currentSubFrame;
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        bool outputState = frameBufs[currentFrameBuf][i] & mask;
        
        digitalWriteFast(DATA_PIN, outputState);
        digitalWriteFast(CLOCK_PIN, HIGH);
        digitalWriteFast(CLOCK_PIN, LOW);  
    }

    digitalWriteFast(LATCH_PIN, HIGH);
    digitalWriteFast(LATCH_PIN, LOW);
}

void cycleMode() {
  mode++;
  if (mode >= NUM_MODES) {
    mode = 0;
  }
}

void cycleSpeed() {
    speedBase++;
    if (speedBase > 16) {
        speedBase = 1;
    }
    speed = 63 * speedBase;
}

void fullOn(uint32_t ledArrPtr[]) {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        ledArrPtr[i] = 0xFFFF;
    }
}

void fullOff(uint32_t ledArrPtr[]) {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        ledArrPtr[i] = 0;
    }
}


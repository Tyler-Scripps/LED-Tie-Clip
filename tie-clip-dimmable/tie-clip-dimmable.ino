// defines for attiny_timerinterrupt.h
// Select USING_HALF_CLOCK == true for  10/ 8MHz to Timer TCBx => shorter timer, but better accuracy
#define USING_HALF_CLOCK true

// use ITimer1 if ITimer0 is used by millis() Servo or tone()
#define USE_TIMER_0 true
#define CurrentTimer ITimer0

#include "ATtiny_TimerInterrupt.h"

#include "dkr.h"

#define DATA_PIN 5
#define CLOCK_PIN 4
#define OE_PIN 6
#define LATCH_PIN 7
#define BUTTON1_PIN 0
#define BUTTON2_PIN 10

#define NUM_LEDS 32
#define NUM_MODES 6

uint16_t frameBufs[2][NUM_LEDS] = {0};

volatile uint16_t currentSubFrame = 0;

volatile bool currentFrameBuf = 0;
volatile bool doneCalculating = false;

uint8_t brightness = 8;

unsigned long lastButton1 = 0;
unsigned long lastButton2 = 0;

uint8_t mode = 0;
uint16_t speed = 125;
uint8_t speedBase = 1;

DKR dkr;

void setup() {
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(OE_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW);

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2, CHANGE);

    displayLeds();
    analogWrite(OE_PIN, 255 - brightness);

    dkr.init(NUM_LEDS);

    CurrentTimer.init();
    CurrentTimer.attachInterruptInterval(31, displayLeds);
}

void loop() {
    if (!doneCalculating) {
        dkr.calculateNextFrame(frameBufs[!currentFrameBuf]);
    }
}

void handleButton1() {
    if (millis() - lastButton1 < 500) {
    return;
    }
    lastButton1 = millis();
    fullOn(frameBufs[currentFrameBuf]);
    displayLeds();
    delay(10000);
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
    delay(10000);
    fullOff(frameBufs[currentFrameBuf]);
    displayLeds();
    cycleSpeed();
}

void displayLeds() {
    currentSubFrame++;

    if (currentSubFrame >= NUM_LEDS && doneCalculating) {   // finsished frame and next is done calculating
        currentSubFrame = 0;
        currentFrameBuf = !currentFrameBuf;
        doneCalculating = false;
    } else if (currentSubFrame >= NUM_LEDS) {   // finished frame but next is still calculating
        currentSubFrame = 0;
    }

    uint16_t mask = 1 << currentSubFrame;
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        
        digitalWrite(DATA_PIN, (bool)(frameBufs[currentFrameBuf][i] & mask));
        digitalWrite(CLOCK_PIN, HIGH);
        digitalWrite(CLOCK_PIN, LOW);  
    }

    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW);
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

void fullOn(uint16_t ledArrPtr[]) {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        ledArrPtr[i] = 0xFFFF;
    }
}

void fullOff(uint16_t ledArrPtr[]) {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        ledArrPtr[i] = 0;
    }
}


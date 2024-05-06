#include <ATtiny_TimerInterrupt.h>
#include <ATtiny_TimerInterrupt.hpp>
#include <ATtiny_ISR_Timer.h>
#include <ATtiny_ISR_Timer.hpp>

#define DATA_PIN 5
#define CLOCK_PIN 4
#define OE_PIN 6
#define LATCH_PIN 7
#define BUTTON1_PIN 0
#define BUTTON2_PIN 10

#define NUM_LEDS 32

#define NUM_MODES 6

#define NUM_PTRS 2

uint8_t* ledArrPtrs[NUM_PTRS];
uint8_t currLedArr = 0;

uint8_t arrSize;

uint8_t brightness = 8;

unsigned long lastButton1 = 0;
unsigned long lastButton2 = 0;

uint8_t mode = 0;
uint16_t speed = 125;
uint8_t speedBase = 1;

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

    // determine number of bytes based on number of leds
    arrSize = NUM_LEDS/8;
    if (NUM_LEDS % 8 != 0)
    {
        arrSize++;
    }

    for(uint8_t i = 0; i < NUM_PTRS; i++) {
        ledArrPtrs[i] = new uint8_t[arrSize];
    }

    for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArrPtrs[0] = 0;
    }
    displayLeds(ledArrPtrs[0]);
    analogWrite(OE_PIN, 255 - brightness);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void handleButton1() {
    if (millis() - lastButton1 < 500) {
    return;
    }
    lastButton1 = millis();
    fullOn(ledArrPtrs[currLedArr]);
    displayLeds(ledArrPtrs[currLedArr]);
    delay(10000);
    fullOff(ledArrPtrs[currLedArr]);
    displayLeds(ledArrPtrs[currLedArr]);
    cycleMode();
}

void handleButton2() {
    if (millis() - lastButton2 < 500) {
    return;
    }
    lastButton2 = millis();
    fullOn(ledArrPtrs[currLedArr]);
    displayLeds(ledArrPtrs[currLedArr]);
    delay(10000);
    fullOff(ledArrPtrs[currLedArr]);
    displayLeds(ledArrPtrs[currLedArr]);
    cycleSpeed();
}

void displayLeds(uint8_t* ledArrPtr) {
    for (int8_t i = arrSize-1; i >= 0; i--)
    {
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ledArrPtr[i]);
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

void fullOn(uint8_t* ledArrPtr) {
    for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArrPtr[i] = 0xFF;
    }
}

void fullOff(uint8_t* ledArrPtr) {
    for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArrPtr[i] = 0;
    }
}


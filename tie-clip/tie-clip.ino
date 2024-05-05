#define DATA_PIN 5
#define CLOCK_PIN 4
#define OE_PIN 6
#define LATCH_PIN 7
#define BUTTON1_PIN 0
#define BUTTON2_PIN 10

#define NUM_LEDS 32

#define NUM_MODES 6

uint8_t *ledArr;
uint8_t arrSize;

uint8_t brightness = 8;

unsigned long lastButton1 = 0;
unsigned long lastButton2 = 0;

uint8_t mode = 0;
// cycle delay in msbitWrite(ledArr[i], j, alternateState);
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
  
    ledArr = new uint8_t[arrSize];

    for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArr[i] = 0;
    }
    displayLeds();
    analogWrite(OE_PIN, 255 - brightness);
}

void loop() {
    switch (mode) {
      case 0:
        knightRider();
        break;
      case 1:
        count();
        break;
      case 2:
        sideScroll();
        break;
      case 3:
        twinkle();
        break;
      case 4:
        alternate();
        break;
      default:
        fullOff();
    }
    displayLeds();
    delay(speed);

}

void handleButton1() {
  if (millis() - lastButton1 < 500) {
    return;
  }
  lastButton1 = millis();
  fullOn();
  displayLeds();
  delay(10000);
  fullOff();
  displayLeds();
  cycleMode();
}

void handleButton2() {
  if (millis() - lastButton2 < 500) {
    return;
  }
  lastButton2 = millis();
  fullOn();
  displayLeds();
  delay(10000);
  fullOff();
  displayLeds();
  cycleSpeed();
}


void displayLeds() {
    for (int8_t i = arrSize-1; i >= 0; i--)
    {
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ledArr[i]);
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

uint8_t scrollPos = 0;
void sideScroll() {
    
    uint8_t tempByte = 0;
    bitSet(tempByte, scrollPos);
    for (uint8_t i = 0; i < arrSize; i++)
    {
      ledArr[i] = tempByte;
    }
    scrollPos++;
    if (scrollPos >= 8)
    {
        scrollPos = 0;
    }
}

uint32_t currCount = 0;
void count(){
  ledArr[3] = (currCount >> 24) & 0xFF; // Most significant byte
  ledArr[2] = (currCount >> 16) & 0xFF;
  ledArr[1] = (currCount >> 8) & 0xFF;
  ledArr[0] = currCount & 0xFF; // Least significant byte
  currCount++;
}

void fullOn() {
  for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArr[i] = 0xFF;
    }
}

void fullOff() {
  for (uint8_t i = 0; i < arrSize; i++)
    {
        ledArr[i] = 0;
    }
}

uint8_t knPos = 0;
bool knDir = 1;
void knightRider(){
  if (knDir) {
    knPos++;
  } else {
    knPos--;
  }

  //switch direction at ends
  if (knPos == 0 || knPos == NUM_LEDS - 1) {
    knDir = !knDir;
  }

  // empty led array
  for (uint8_t i = 0; i < arrSize; i++)
  {
    ledArr[i] = 0;
  }

  bitSet(ledArr[knPos/8], (knPos%8));
  // ledArr[knPos / 8] = 1 << (knPos % 8);
}

void twinkle() {
  for (uint8_t i = 0; i < arrSize; i++) {
    ledArr[i] = random(255);
  }
}

bool alternateState = 0;
void alternate() {
  for (uint8_t i = 0; i < arrSize; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      if (j%2) {
        bitWrite(ledArr[i], j, alternateState);
      } else {
        bitWrite(ledArr[i], j, !alternateState);
      }
      
    }
  }
  alternateState = !alternateState;
}

void testPattern(uint8_t mode) {
  uint8_t testPat;
  for (uint8_t i = 0; i < arrSize; i++)
  {
    ledArr[i] = 0;
  }
  switch (mode) {
    case 0:
      testPat = 0b10011100;
      for (uint8_t i = 0; i < arrSize; i++)
      {
        ledArr[i] = testPat;
      }
      break;
    case 1:
      testPat = 0b10000001;
      for (uint8_t i = 0; i < arrSize; i++)
      {
        ledArr[i] = testPat;
      }
      break;
    case 2:
      testPat = 0b00000011;
      for (uint8_t i = 0; i < arrSize; i++)
      {
        ledArr[i] = testPat;
      }
      break;
    case 3:
      ledArr[0] = 0b00000111;
      break;
    case 4:
      ledArr[0] = 0b00000001;
      ledArr[1] = 0b00000001;
      ledArr[2] = 0b00000001;
      ledArr[3] = 0b11000000;
      break;
    case 5:
      ledArr[3] = 0b11000000;
      break;
    case 6:
      testPat = 0b11100000;
      for (uint8_t i = 0; i < arrSize; i++)
      {
        ledArr[i] = testPat;
      }
      break;
  }
}
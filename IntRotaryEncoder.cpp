#include <Arduino.h>
#include "IntRotaryEncoder.h"


// interrupt 0 services pin 3
#define INT0 0
#define PIN0 2

// interrupt 1 services pin 4
#define INT1 1
#define PIN1 3

volatile int _pos1 = 0;
volatile int _pos2 = 0;
volatile int _lastPosition = 0;

// use separate counts because we handle two interrupts here
// and it appears they can overlap.

static void intRotaryEncoderISR()
{
  static byte lastData = 0x3;
  byte data = (PIND >> 2) & 0x3; // pin3 and pin2 data
  byte trigger = (lastData ^ data);
  lastData = data;
  
  if (trigger == 0x1) { // pin0 changed
    _pos1 += ((data & 1) ^ data>>1) ? 1 : -1;
  } else if (trigger == 0x2) { // pin1 changed
    _pos2 += ((data & 1) ^ data>>1) ? -1 : 1;  
  }
}
  

IntRotaryEncoder::IntRotaryEncoder()
{
  pinMode(PIN0, INPUT);
  digitalWrite(PIN0, HIGH);  // enable internal pull-up resistors to avoid floating inputs

  pinMode(PIN1, INPUT);
  digitalWrite(PIN1, HIGH);  // enable internal pull-up resistors to avoid floating inputs

  attachInterrupt(INT0, intRotaryEncoderISR, CHANGE);
  attachInterrupt(INT1, intRotaryEncoderISR, CHANGE);  
}

int IntRotaryEncoder::read()
{
  int position = _pos1 + _pos2;
  int delta = position - _lastPosition;
  _lastPosition = position;
  return delta;
}



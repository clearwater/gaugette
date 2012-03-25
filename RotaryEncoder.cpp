#include <Arduino.h>
#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(unsigned char pin1, unsigned char pin2)
{
  this->pin1 = pin1;
  this->pin2 = pin2;
  pinMode(pin1, INPUT);
  digitalWrite(pin1, HIGH);  // enable internal pull-up resistors to avoid floating inputs
  pinMode(pin2, INPUT);
  digitalWrite(pin2, HIGH);
}

int RotaryEncoder::read()
{
  int delta = 0;
  static unsigned char lastData = 0;
  unsigned char data = (PIND >> 2) & 0x3;
  unsigned char change = data ^ lastData;
  
  if (change & 0x1) { // pin0 changed
    delta += ((data & 1) ^ data>>1) ? +1 : -1;
  } else if (change & 0x2) { // pin1 changed
    delta += ((data & 1) ^ data>>1) ? -1 : +1;    
  }
  lastData = data;
  return delta;
} 


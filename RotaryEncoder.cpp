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

char RotaryEncoder::read()
{
  char result = 0;
  static unsigned char lastPinc = 0;
  unsigned char pinc = PINC & 0x3;
  unsigned char change = pinc ^ lastPinc;

//  if ((change+1) | 0x3) {  // equivalent, just if change==1 or ==2
  if ((change ^ (change>>1)) & 0x1) {
    unsigned char dir = (lastPinc >> 1) ^ (pinc & 0x1);
    result = dir ? -1 : 1;    
    Serial.println(result);
    lastPinc=pinc;
  }
  return result;
} 

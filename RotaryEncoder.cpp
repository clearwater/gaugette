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

/*
char RotaryEncoder::read()
{
  char result = 0;
  static unsigned char lastPinc = 0;
  unsigned char pinc = (PIND >> 3) & 0x3;
  //unsigned char pinc = PINC & 0x3;
  unsigned char change = pinc ^ lastPinc;
  
  //  if ((change+1) | 0x3) {  // equivalent, just if change==1 or ==2
  if ((change ^ (change>>1)) & 0x1) {
    unsigned char dir = (lastPinc >> 1) ^ (pinc & 0x1);
    result = dir ? -1 : 1;    
    //Serial.println(result);
    lastPinc=pinc;
  }
  return result;
} 
*/

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

/*
if interrupt driven we split the handler into to parts:
 - bit 0 changes
 - bit 1 changes
 
// 1 1  << moving forward
// 1 0
// 0 0
// 0 1
// 1 1
//
// 1 1  << moving backwards
// 0 1
// 0 0
// 1 0
// 1 1
//

if bit 0 change ^ (bit0 ^ bin1)

byte pinc = PINC & 0x3
delta += (pinc&1 ^ pinc>>1) ? 1 : -1;




*/

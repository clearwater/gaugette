#include <Arduino.h>
#include "LCD.h"

LCD::LCD(unsigned char pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
}

void LCD::set(unsigned char value)
{
  analogWrite(pin, value);
}

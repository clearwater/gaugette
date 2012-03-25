
#include <Arduino.h>
#include "Switch.h"

Switch::Switch(unsigned char pin)
{
  this->pin = pin;
  this->set = false;
  pinMode(pin, INPUT);           // set pin to input
  digitalWrite(pin, HIGH);       // turn on pullup resistors
}

boolean Switch::isSet()
{
  return 0==digitalRead(this->pin);
}

// if changed returns true, check sw.set for new value
boolean Switch::changed()
{
  boolean wasSet = this->set;
  this->set = this->isSet();
  return (this->set != wasSet);
}

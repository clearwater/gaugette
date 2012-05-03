#include <Arduino.h>
#include "LED.h"

LED::LED(unsigned char pin)
{
  this->pin = pin;
  this->time0 = 0;
  this->target = 0;
  this->value = 0;
  this->period = 0;  // instant
  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
}

void LED::set(unsigned char value)
{
  if (this->period==0) {
    this->value = this->target = value;
    analogWrite(pin, this->value);
  } else {
    this->time0 = millis();
    this->target = value;
  }
}

void LED::update()
{
  if (this->value != this->target) {
     unsigned long delta = millis() - time0;
     unsigned char newValue;
     if (delta>=period) {
       Serial.print(delta);
       Serial.println(" done");
       newValue = this->value = this->target; // done
     } else {
       int dv = (int)target - (int)value;  
       // cast to long because dv * d will exceed 16-bit signed int.
       int dvt = (long)dv * (long)delta / (long)period;
       newValue = (int)value + dvt;
     }
     analogWrite(pin, newValue);
  }
}

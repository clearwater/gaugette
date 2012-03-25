#include <Arduino.h>
#include "LED.h"

LED::LED(unsigned char pin)
{
  this->pin = pin;
  this->target = 0;
  this->value = 0;
  this->speed = 0;  // instant
  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
}

void LED::set(unsigned char value)
{
  if (this->speed==0) {
    this->value = this->target = value;
    analogWrite(pin, this->value);
  } else {
    this->target = value;
  }
}

void LED::update()
{
  if (this->value != this->target) {
     if (this->value < this->target) {
       unsigned char delta = this->target-this->value;
       if (delta > this->speed) delta = this->speed;
       this->value+=delta;
     } else {
       unsigned char delta = this->value-this->target;
       if (delta > this->speed) delta = this->speed;
       this->value-=delta;
     }
     Serial.println(this->value);
     analogWrite(pin, this->value);
  }
}

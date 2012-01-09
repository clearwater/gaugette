#include <Arduino.h>

#include "SwitecX25.h"

SwitecX25::SwitecX25(unsigned int steps, unsigned char pins[4])
{
  this->currentState = 0;
  this->steps = steps;
  this->pins = pins;
  for (int i=0;i<pinCount;i++) {
    pinMode(pins[i], OUTPUT);
  }
  currentStep = 0;
  targetStep = 0;
  stopped = true;
}

void SwitecX25::writeIO()
{
  byte mask = SwitecX25StateMap[currentState];  
  for (int i=0;i<pinCount;i++) {
    digitalWrite(pins[i], mask & 0x1);
    mask >>= 1;
  }
}

void SwitecX25::stepUp()
{
  currentStep++;
  currentState = (currentState + 1) % stateCount;
  writeIO();
}

void SwitecX25::stepDown()
{
  currentStep--;
  currentState = (currentState + 5) % stateCount;
  writeIO();
}

void SwitecX25::zero()
{
  for (unsigned int i=0;i<steps;i++) {
    stepUp();
    delayMicroseconds(800);
  }
  for (unsigned int i=0;i<steps;i++) {
    stepDown();
    delayMicroseconds(800);    
  }
  currentStep = 0;
  targetStep = 0;
  stopped = true;
}

void SwitecX25::advance()
{
  if (targetStep > currentStep) {
    stepUp();
    stopped = false;
  } else if (targetStep < currentStep) {
    stepDown();
    stopped = false;
  } else {
    stopped = true;
  }
  time0 = micros();
}

void SwitecX25::setPosition(unsigned int pos)
{
  if (pos > steps) pos = steps;
  targetStep = pos;  
  stopped = false;
  Serial.println(targetStep);
}

void SwitecX25::update()
{
  if (!stopped) {
    unsigned long delta = time0 - micros();
    if (delta >= 1000) {  // hack - magic number
      Serial.println("advance");
      advance();
    }
  }
}



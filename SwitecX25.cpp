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
  microDelay = 1000; // microseconds
  minDelay = 500;
  currentStep = 0;
  targetStep = 0;
  velMin = 10.0f;
  velMax = 2000.0f;
  accel = 400.0f;
  vel = 0.0f;  // current velocity, signed
  
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
  boolean fwd = targetStep > currentStep;

  if (stopped) {
    // starting
    microDelay = maxMicroDelay;
    stopped = false;
  } else {
    // accel
    microDelay = microDelay * 100 / 95;  // hack for acceleration
    if (microDelay < minMicroDelay) microDelay = minMicroDelay;
  }
  
  if (targetStep > currentStep) {
    stepUp();
  } else if (targetStep < currentStep) {
    stepDown();
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
    unsigned long delta = micros() - time0;
    if (delta >= microDelay) {  // hack - magic number
      //Serial.println("advance");
      advance();
    }
  }
}



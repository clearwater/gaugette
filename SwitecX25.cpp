#include <Arduino.h>

#include "SwitecX25.h"

// This table defines the acceleration curve.
// 1st value is the speed step, 2nd value is delay in microseconds
// 1st value in each row must be > 1st value in subsequent row
// 1st value in last row should be == maxVel, must be <= maxVel
unsigned short accelTable[][2] = {
  {   50, 4000}, // v, uDelay
  {  100, 2000},
  {  500, 1000}
};

SwitecX25::SwitecX25(unsigned int steps, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4)
{
  this->currentState = 0;
  this->steps = steps;
  this->pins[0] = pin1;
  this->pins[1] = pin2;
  this->pins[2] = pin3;
  this->pins[3] = pin4;
  for (int i=0;i<pinCount;i++) {
    pinMode(pins[i], OUTPUT);
  }
  
  dir = 0;
  vel = 0; 
  stopped = true;
  currentStep = 0;
  targetStep = 0;
  maxVel = 500;
}
/*
void SwitecX25::setSpeed(float minStepsPerSec, float maxStepsPerSec)
{
  velMin = minStepsPerSec;
  velMax = maxStepsPerSec;
  maxMicroDelay = 1000000.0f / minStepsPerSec;
  minMicroDelay = 1000000.0f / maxStepsPerSec;
}

void SwitecX25::setDelay(int minMicroSec, int maxMicroSec)
{
  minMicroDelay = minMicroSec;
  maxMicroDelay = maxMicroSec;
  velMin = 1000000.0f / (float)maxMicroDelay;
  velMax = 1000000.0f / (float)minMicroDelay;
}

void SwitecX25::setAccel(float accelStepsPerSecPerSec, float decelStepsPerSecPerSec)
{
  accel = accelStepsPerSecPerSec;
  decel = decelStepsPerSecPerSec;
}
*/
void SwitecX25::writeIO()
{
  // State  3 2 1 0   Value
  // 0      1 0 0 1   0x9
  // 1      0 0 0 1   0x1
  // 2      0 1 1 1   0x7
  // 3      0 1 1 0   0x6
  // 4      1 1 1 0   0xE
  // 5      1 0 0 0   0x8
  static byte stateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};

  byte mask = stateMap[currentState];  
  for (int i=0;i<pinCount;i++) {
    digitalWrite(pins[i], mask & 0x1);
    mask >>= 1;
  }
}

void SwitecX25::stepUp()
{
  if (currentStep < steps-1) {
    currentStep++;
    currentState = (currentState + 1) % stateCount;
    writeIO();
  }
}

void SwitecX25::stepDown()
{
  if (currentStep > 0) {
    currentStep--;
    currentState = (currentState + 5) % stateCount;
    writeIO();
  }
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
  vel = 0;
  dir = 0;
}


// decel:
// know v
// know d
// v = d / t
// d = t * v
// t = d / v
// v2 = v1 + a.t
// v2 = 0
// 0 = v + a.t
// v = a.t
// a = v / t

// start to stop from v in d takes time st = d / v/2 = 2d / v 
// decel rate is v / st = v / (2d / v) = 1 / 2d 


void SwitecX25::advance()
{
  time0 = micros();
  
  // detect stopped state
  if (currentStep==targetStep && vel==0) {
    stopped = true;
    dir = 0;
    return;
  }
  
  // if stopped, determine direction, start moving, quit
  if (vel==0) {
    dir = currentStep<targetStep ? 1 : -1;
    vel = 1;
    return;
  }
  
  if (dir>0) {
    stepUp();
  } else {
    stepDown();
  }
  
  // determine delta, number of steps in current direction to target.
  // may be negative if we are headed away from target
  int delta = dir>0 ? targetStep-currentStep : currentStep-targetStep;
  
  if (delta>0) {
    // case 1 : moving towards target (maybe under accel or decel)
    if (delta <= vel) { // time to declerate
      //Serial.println("decel");
      vel--;
    } else if (vel < maxVel) {
      //Serial.println("accel");
      vel++;
    } else {
      //Serial.println("top speed");
      // at full speed - stay there
    }
  } else {
    // case 2 : at or moving away from target (slow down!)
    vel--;
  }
    
  // vel now defines delay
  unsigned char i = 0;
  // this is why vel must not be greater than the last vel in the table.
  while (accelTable[i][0]<vel) {
    i++;
  }
  microDelay = accelTable[i][1];
  //Serial.print(vel);
  //Serial.print(" ");
  //Serial.println(microDelay);
}

void SwitecX25::setPosition(unsigned int pos)
{
  if (pos > steps-1) pos = steps-1;
  targetStep = pos;
  stopped = false;
  //Serial.println(targetStep);
}


void SwitecX25::update()
{
  if (!stopped) {
    unsigned long delta = micros() - time0;
    if (delta >= microDelay) {
      advance();
    }
  }
}



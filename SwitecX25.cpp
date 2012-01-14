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
  microDelay = 0; // microseconds
  minMicroDelay = 500;    // fast limit
  maxMicroDelay = 5000;  // slow limit
  microDelayAccel = 950;  // accel factor /1000
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
  stopped = true;
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

  // can't use statics for real - make these instance vars
  static float vel = 0.0f;      // steps per sec
  static float accel = 5000.0f;  // steps per sec per sec
  static float decel = 2000.0f; // steps per sec per sec
  static float velMin = 500.0f;
  static float velMax = 1000000.0f/(float)minMicroDelay;
  static float dt = 0.0f;
  static float M = 1000000.0f;
  
  boolean fwd = targetStep > currentStep;
  if (targetStep == currentStep) {
    stopped = true;
    microDelay = 0;
    return;
  }

  if (microDelay == 0) {
    vel = fwd ? velMin : -velMin;    
    stopped = false;
  } else {
    // accel towards the target
    float a = fwd ? accel : -accel;
   
    // determine if we should start breaking
    //Serial.print(vel);

    float d = (float)targetStep - (float)currentStep;  // how many steps to travel
    //Serial.print(" d=");
    //Serial.print(d);
    //Serial.print(" v=");
    //Serial.print(vel);
    if (d * vel > 0) { // signs are same, so travelling towards target
      float requiredDec = vel / d * vel / 2; // decel required to stop      
      //Serial.print(" r=");
      //Serial.println(requiredDec);
      if (fabs(requiredDec) >= decel) {
        a = -requiredDec;
        //Serial.print(" brakes");
      }
    }
    //Serial.print(" a=");
    //Serial.println(a);
    vel = vel + a * dt;
    if (vel>velMax) vel=velMax;
    if (vel<-velMax) vel=-velMax;
  }
  dt = 1.0f / fabs(vel);  // time for 1 step at vel
  
  microDelay = dt * 1000000.0f;
  //Serial.print(vel);
  //Serial.print(" ");
  //Serial.println(microDelay);
  if (microDelay < minMicroDelay) microDelay = minMicroDelay;
  if (microDelay > maxMicroDelay) microDelay = maxMicroDelay; 

  if (vel > 0) {
    stepUp();
  } else if (vel < 0) {
    stepDown();
  } else {
    microDelay = 0; // signal to self that it was stopped 
    stopped = true;
  }
}

void SwitecX25::setPosition(unsigned int pos)
{
  if (pos > steps-1) pos = steps-1;
  targetStep = pos; 
  stopped = false;
  Serial.println(targetStep);
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



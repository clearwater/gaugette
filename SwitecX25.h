#ifndef SwitecX25_h
#define SwitecX25_h

class SwitecX25
{
 public:
   static const unsigned char pinCount = 4;
   static const unsigned char stateCount = 6;
   unsigned char *pins;
   unsigned char currentState;
   unsigned int currentStep;
   unsigned int targetStep;
   unsigned int steps;
   unsigned long time0;  // time when we entered this state
   boolean stopped;
   unsigned int microDelay;
   unsigned int minMicroDelay;
   unsigned int maxMicroDelay;
   // REVISIT - with care this could all be done in integer arithmetic
   float vel;      // steps per sec
   float accel;  // steps per sec per sec
   float decel; // steps per sec per sec
   float velMin;
   float velMax;
   float dt;
   
   SwitecX25(unsigned int steps, unsigned char pins[4]);
   void setSpeed(float minStepsPerSec, float maxStepsPerSec);
   void setDelay(int minMicroSec, int maxMicroSec);
   void setAccel(float accelStepsPerSecPerSec, float decelStepsPerSecPerSec);
   
   void stepUp();
   void stepDown();
   void zero();
   void setPosition(unsigned int pos);
   void update();
  
  
 private:
   void advance();
   void setDelay();
   void writeIO();
};


#endif


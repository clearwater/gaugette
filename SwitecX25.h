#ifndef SwitecX25_h
#define SwitecX25_h

class SwitecX25
{
 public:
   static const unsigned char pinCount = 4;
   static const unsigned char stateCount = 6;
   unsigned char pins[pinCount];
   unsigned char currentState;
   unsigned int currentStep;
   unsigned int targetStep;
   unsigned int steps;
   unsigned long time0;  // time when we entered this state
   unsigned int microDelay;
   unsigned int maxVel;
   unsigned int vel;  // velocity surrogate (measured in steps of accel)
   char dir;  // direction -1,0,1  
   boolean stopped;
   
   SwitecX25(unsigned int steps, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4);
  
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


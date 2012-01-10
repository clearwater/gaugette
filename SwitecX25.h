#ifndef SwitecX25_h
#define SwitecX25_h

// State  3 2 1 0   Value
// 0      1 0 0 1   0x9
// 1      0 0 0 1   0x1
// 2      0 1 1 1   0x7
// 3      0 1 1 0   0x6
// 4      1 1 1 0   0xE
// 5      1 0 0 0   0x8

static byte SwitecX25StateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//static byte SwitecX25StateMap[] = {0x8, 0xE, 0x6, 0x7, 0x1, 0x9};

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
   
   SwitecX25(unsigned int steps, unsigned char pins[4]);
   void stepUp();
   void stepDown();
   void zero();
   void setPosition(unsigned int pos);
  void update();
  
 private:
   void advance();
   void writeIO();
};


#endif


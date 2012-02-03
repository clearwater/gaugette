#ifndef _IntRotaryEncoder_h_
#define _IntRotaryEncoder_h_

// Since the standard arduino only has interrupts for pins 2 and 3
// the rotary encoder MUST be attached to those two pins.

class IntRotaryEncoder
{
  public: 
    IntRotaryEncoder();
    int read();  // returns signed delta
};

#endif


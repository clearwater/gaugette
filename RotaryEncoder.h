#ifndef RotaryEncoder_h
#define RotaryEncoder_h

// Sequence moving forward:
// 1 1  << stable state
//
// 1 1  << moving forward
// 1 0
// 0 0
// 0 1
// 1 1
//
// 1 1  << moving backwards
// 0 1
// 0 0
// 1 0
// 1 1
//
// FWD: 3 1 0 2 3
// was 0 now 1 : fwd
// was 1 now 3 : fwd
// was 2 now 0 : fwd
// was 3 now 2 : fwd
// 
// REV: 3 2 0 1 3
// was 0 now 2 : rev
// was 1 now 0 : rev
// was 2 now 3 : rev
// was 3 now 1 : rev
//
// 


class RotaryEncoder
{
  public:
    unsigned char pin1;
    unsigned char pin2;
  
    RotaryEncoder(unsigned char pin1, unsigned char pin2);
    char read();
};

#endif

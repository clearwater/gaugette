#ifndef LED_h
#define LED_h

class LED
{
  public:
  unsigned char pin;
  unsigned char target;
  unsigned char value;
  unsigned long period;
  unsigned long time0;
  LED(unsigned char pin);
  void set(unsigned char value);
  void update();
};

#endif


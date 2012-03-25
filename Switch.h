#ifndef Switch_h
#define Switch_h

class Switch
{
  public:
  unsigned char pin;
  boolean set;
  Switch(unsigned char pin);
  boolean isSet();
  boolean changed();
};

#endif


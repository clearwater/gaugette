#ifndef LCD_h
#define LCD_h

class LCD
{
  public:
  unsigned char pin;
  LCD(unsigned char pin);
  void set(unsigned char value);
};

#endif


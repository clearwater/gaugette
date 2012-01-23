Project Blog
============
Full details of this project can be found at [the Gaugette Blog](http://clearwater.github.com/gaugette/).

Gaugette and SwitecX25 library
==============================
There are two pieces of code in this repository.  

Using The SwitecX25 Library
---------------------------
The SwitecX25 library is a general purpose library for integrating with your own Arduino sketches.

```C++
#import "SwitecX25.h"

// declare motor1 on pins 8-11
unsigned char motor1pins[4] = {8,9,10,11};
SwitecX25 motor1(315 * 3, motor1pins);

// declare motor2 on pins 4-7
unsigned char motor2pins[4] = {4,5,6,7};
SwitecX25 motor2(315 * 3, motor2pins);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Gaugette!");
  // run both motors against stops to re-zero
  motor1.zero();   // this is a slow, blocking operation
  motor2.zero();  
}

void loop(void) {
  // update motors frequently
  motor1.update();
  motor2.update();
  // do stuff...
  motor1.setPosition(427);
  motor2.setPosition(427);
}

```

Using Gaugette
==============
Gaugette.ino is an Ardiuno sketch that uses a simple serial
command language to control one or more X25's.
The following ruby sample code sends command sequences
to the attached Arduino from a host computer.  The clients directory includes
a complete example that showns bandwidth in and out from a pfSense firewall
on two analog gauges.

```ruby
require 'gaugette'

# open serial connection to Arduino running gaugette.ino
@gaugette = Gaugette.new("/dev/tty.usbmodem24411")

# zero and configure each motor
[0,1].each do |i|
  @gaugette.zero(i)
  @gaugette.accel(i,500, 500)
  @gaugette.delay(i,400, 800)

# move motors in sync
v = 0
6.times do |i|
  @gaugette.set(1, v)
  @gaugette.set(0, v)
  sleep 1
  v = 1-v
end
```












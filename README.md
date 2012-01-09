Driving Switec X25 Instrument Steppers With an Arduino
======================================================

When I began researching motors to build some custom analogue
guages, I noticed that the Switec X25 motors used
in GM instrument gauges were incredibly cheap, readily available on eBay,
and some sources suggested they might interface directly
to the digital output lines on the Arduino without the need
for an external H-bridge or driver circuit.

The motors have 6 steps per rotation, plus an 180:1 gearbox, which
yields 1/3 degree of shaft rotation per step, which is great.

So I ordered 6 motors from eBay and this project contains the code
I'm using to turn these into guages.

Driving Directly from the Arduino
=================================

The data sheet says 
_"The motor can operate directly from a numerical, i.e. digital, driving signal to move and position a pointer to visualise any parameter required"_
The motors draws something under 20mA at 5V, 
and the Arduino 5v digital output pins can
[sink up to 40mA each.](http://www.arduino.cc/en/Main/arduinoBoardUno)

There are a few articles about interfacing these motors to an Arduino.  
This [introduction](http://www.mycockpit.org/forums/content.php/355-An-Easy-Approach-to-an-Analog-Gauge)
is great, but the only article I found reporting success without using
an intermediate controller was [this one](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=126097896)
and the thread ends with the first hints of success.  The question about
direct drive seems to be will the inductance of the motor cause large currents
which cook the Arduino?  So I figure it's worth a shot and Arduinos are cheap.











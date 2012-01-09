Driving Switec X25 Instrument Steppers With an Arduino
======================================================

When I began researching motors to build some custom analogue
guages, I noticed that the Switec X25 series miniature stepper motors used
in GM instrument gauges were incredibly cheap, readily available on eBay,
and some sources suggested they might interface directly
to the digital output lines on the Arduino without the need
for an external H-bridge or driver circuit.

The motors have 6 steps per rotation, plus an 180:1 gearbox, which
yields 1/3 degree of shaft rotation per step, which is great.

I ordered 6 motors from eBay and this project contains the code
I'm using to drive them.

Switec X12 Quad Driver Chip
===========================
The X25 data sheet describes the X12 family of driver chips which allow
up to 4 Switech motors to be controlled over a 3-wire serial interface.
I like the look of these for keeping the I/O count down, and offloading
a bunch of processing from the Arduino.

However I have been unable to find a supplier for X12 quad driver chips.
It seems that they were available until recently
from Swatch subsidiary Microcomponents Ltd. at microcomponents.ch,
but after Swatch 
[sold the stepping motor business](http://www.swatchgroup.com/en/services/archive/2009/swatch_group_sale_of_microcomponents_automotive_business_activities_to_juken_technology)
to Singapore-based [Juken Techonology](www.jukenswisstech.com) in 2009, 
that website disappeared.

Driving Directly from the Arduino
=================================

The data sheet says 
_"The motor can operate directly from a numerical, i.e. digital, driving signal to move and position a pointer to visualise any parameter required."_

So that sounds promising.  It appears the motors draws 15-20mA per winding at 5V,
and the Arduino 5v digital output pins can
[source or sink up to 40mA each.](http://www.arduino.cc/en/Main/arduinoBoardUno)

There are a few articles about interfacing these motors to an Arduino.
This [discussion of the motors by Mike Powell](http://www.mycockpit.org/forums/content.php/355-An-Easy-Approach-to-an-Analog-Gauge) is excellent, but the author expresses concerns about driving inductive loads directly
from the microcontroller and opts for an external L293D driver.

The only material I found reporting success without using
a separate controller chip is
[this thread from tobycatlin](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1260978962)
in the Arduino forum, but the thread ends unsatisfyingly at the first hints of success.
Maybe it went up in smoke?

Meh, I figure it's worth a shot.  Life is short and Arduinos are cheap.

The short answer is yes, it works.
I've had very good success driving the motor directly from the Arduino.
It takes 4 IO pins.  In theory you could use 3 because the 2nd and 3rd pin
always have the same output voltage, but unless I run out of pins I figure
it's best not to tempt the over-current gods.

Resources
=========

1. [M-S Motor X25 Data Sheet](http://forums.freescale.com/freescale/attachments/freescale/16BITCOMM/9620/1/X25_xxx_01_SP_E-1.pdf)












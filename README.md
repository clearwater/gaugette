Dependences
===========

This code depends on the SwitecX25 library which is now housed in [a separate repository](https://github.com/clearwater/SwitecX25).

Project Blog
============
Full details of this project can be found at [the Gaugette Blog](http://clearwater.github.com/gaugette/).

Using this Code
===============

Gaugette.ino is an Ardiuno sketch that uses a simple serial
command language to control one or more analog gauges.
The following ruby sample code sends command sequences
to the attached Arduino from a host computer.  The clients directory includes
a complete example that showns bandwidth in and out from a pfSense firewall
on two analog gauges.

```ruby
require 'gaugette'

# open serial connection to Arduino running gaugette.ino
@gaugette = Gaugette.new("/dev/tty.usbmodem24411")

# zero and configure each motor
[0,1].each {|i| @gaugette.zero(i)}

# move motors in sync
v = 0
6.times do |i|
  @gaugette.set(1, v)
  @gaugette.set(0, v)
  sleep 1
  v = 1-v
end
```

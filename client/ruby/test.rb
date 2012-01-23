# Exercise the gaugette device

require 'rubygems'
require 'serialport'
require 'gaugette'
require 'pp'

@gaugette = Gaugette.new("/dev/tty.usbmodem24411")
@gaugette.zero(0)
@gaugette.zero(1)

# Both motors move in sync
v = 0
6.times do |i|
  @gaugette.set(1, v)
  @gaugette.set(0, v)
  sleep 1
  v = 1-v
end

# Center both motors
@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)
sleep 2.5

# Motor 1 leads 0 by 0.5 seconds
6.times do |i|
  @gaugette.set(1, v)
  sleep 0.5
  @gaugette.set(0, v)
  sleep 0.5
  v = 1-v
end

# Center both motors
@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)
sleep 2.5

# Motors in opposite phase
6.times do |i|
  @gaugette.set(0, 1-v)
  @gaugette.set(1, v)
  sleep 1.0
  v = 1-v
end

# Center both motors
@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)




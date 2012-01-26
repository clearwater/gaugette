#!/usr/bin/env ruby
# Exercise the gaugette device

require 'rubygems'
require 'serialport'
require 'gaugette'
require 'pp'

@gaugette = Gaugette.new("/dev/tty.usbmodem24411")
@gaugette.zero(0)
@gaugette.zero(1)

v = 0
5.times do |pattern|
  6.times do |i|
    @gaugette.set(0,v)
    t0 = Time.now
    case pattern
    when 0:
        # in sync
        @gaugette.set(1,v)
    when 1:
        # out of sync
        @gaugette.set(1,1-v)
    when 2:
        # lag by .5 sec
        sleep 0.5
      @gaugette.set(1,v)
    when 3:
        # half amplitude
        @gaugette.set(1,0.25+v/2.0)
    when 4:
        # double time
        @gaugette.set(1,0.40)
      sleep 0.25
        @gaugette.set(1,0.60)
      sleep 0.25
        @gaugette.set(1,0.40)
      sleep 0.25
        @gaugette.set(1,0.60)
    end
    v = 1-v
    t1 = Time.now
    sleep 1.0 - (t1-t0)  # 1 second cycles
  end
  @gaugette.set(0,0.5)
  @gaugette.set(1,0.5)
  sleep 1
end
    
raise 'done'
  
# Both motors move in sync
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

0



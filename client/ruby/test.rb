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
        # forced
        @gaugette.set(1,v)
        sleep 0.25
        @gaugette.set(1,v*0.75+(1-v)*0.25)
        sleep 0.25
        @gaugette.set(1,0.5)
        sleep 0.25
        #@gaugette.set(1,v*0.25+(1-v)*0.75)
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
    

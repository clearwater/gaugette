# appears we can pull
# GET /ifstats.php?if=vr0
# from pfsense
# after authentication

require 'rubygems'
require 'curb'
require 'sqlite3'
require 'database'
require 'serialport'
require 'gaugette'
require 'pp'

@gaugette = Gaugette.new("/dev/tty.usbmodem24411")
@gaugette.zero(0)
@gaugette.zero(1)

v = 0
6.times do |i|
  @gaugette.set(1, v)
  @gaugette.set(0, v)
  sleep 1
  v = 1-v
end

@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)
sleep 2.5

6.times do |i|
  @gaugette.set(1, v)
  sleep 0.5
  @gaugette.set(0, v)
  sleep 0.5
  v = 1-v
end

@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)
sleep 2.5

6.times do |i|
  @gaugette.set(0, 1-v)
  @gaugette.set(1, v)
  sleep 1.0
  v = 1-v
end

@gaugette.set(0, 0.5)
@gaugette.set(1, 0.5)
sleep 2.5



#!/usr/bin/env ruby
# appears we can pull
# GET /ifstats.php?if=vr0
# from pfsense without authentication

require 'rubygems'
require 'curb'
require 'serialport'
require 'gaugette'
require 'pp'

URL = "http://pfsense.clearwater.com.au/ifstats.php?if=vr0"

@curl = Curl::Easy.new()
@curl.enable_cookies = true
@curl.follow_location = false
@curl.url = URL
@gaugette = Gaugette.new("/dev/tty.usbmodem24411")
[0,1].each do |i|
  @gaugette.zero(i)
  @gaugette.accel(i,500, 500)
  @gaugette.delay(i,400, 800)
end

last_time = nil
last_bytes_in = nil
last_bytes_out = nil
interval = 2 # polling interval in seconds 

while true
  @curl.http_get
  time, bytes_in, bytes_out = @curl.body_str.strip.split("|").map{|n| n.to_f}
  if last_time
    delta_time = time - last_time
    bps_in = (bytes_in-last_bytes_in)/delta_time
    bps_out = (bytes_out-last_bytes_out)/delta_time
    #PP.pp [delta_time, bps_in, bps_out]
    @gaugette.set(0, bps_in / 100000.0)
    @gaugette.set(1, bps_out/ 100000.0)
  end
  last_time = time
  last_bytes_in = bytes_in
  last_bytes_out = bytes_out
  sleep interval
end

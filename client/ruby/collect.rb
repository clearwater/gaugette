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

URL = "http://pfsense.clearwater.com.au/ifstats.php?if=vr0"
DB = 'vr0.db'

@curl = Curl::Easy.new()
@curl.enable_cookies = true
@curl.follow_location = false
@curl.url = URL
@db = Database.new(DB)
@gaugette = Gaugette.new("/dev/tty.usbmodem24411")

last_time = nil
last_bytes_in = nil
last_bytes_out = nil

while true
  @curl.http_get
  time, bytes_in, bytes_out = @curl.body_str.strip.split("|").map{|n| n.to_f}
  if last_time
    delta_time = time - last_time
    bps_in = (bytes_in-last_bytes_in)/delta_time
    bps_out = (bytes_out-last_bytes_out)/delta_time
    #PP.pp [delta_time, bps_in, bps_out]
    @gaugette.write(bps_in / 100000.0)
    @db.write(Time.now.to_i, bps_in, bps_out)
  end
  last_time = time
  last_bytes_in = bytes_in
  last_bytes_out = bytes_out
  sleep 1
end
#!/usr/bin/env ruby

# add this file's directory to load path
$:.unshift File.dirname(__FILE__)

require 'rubygems'
require 'curb'
require 'serialport'
require 'gaugette'
require 'pfsense'
require 'pp'

gaugette_device = "/dev/tty.usbmodem411"  # serial port to Arduino
gaugette_steps = 696                      # full-scale deflection
interval = 2                              # polling interval in seconds 
pfsense_host = "pfsense"                  # hostname of firewall
pfsense_interface = "wan"                  # interface on firewall

# read the credentials from a file.  You could just code this as
# pfsense_credentials = ['admin','pfsense'] instead.
pfsense_credentials = File.read(File.join(File.dirname(__FILE__),"pfsense.credentials")).strip.split('/')

puts "Displaying bandwidth from #{pfsense_host}:#{pfsense_interface} on gaugette at #{gaugette_device}"

# initialize pfsense connection
@pfsense = PfSense.new(pfsense_host)
@pfsense.login(*pfsense_credentials)

# initialize gaugette connection
@gaugette = Gaugette.new(gaugette_device, gaugette_steps)
[0,1].each do |i|
  @gaugette.range(i, gaugette_steps) # unnecessary
  @gaugette.zero(i)
end

# The first call to @pfsense.read will return nil,nil.  Subsequent
# calls will return [Bps_in, Bps_out] in BYTES per second.

while true
  bps_in, bps_out = @pfsense.read("wan")
  if bps_in
    kbps_in = bps_in / 1000.0
    @gaugette.set_scaled(0, kbps_in, 100.0)
    @gaugette.lcd(0, kbps_in * 255 / 100.0)
  end
  if bps_out
    kbps_out = bps_out / 1000.0
    @gaugette.set_scaled(1, kbps_out, 100.0)
    @gaugette.lcd(1, kbps_out * 255 / 100.0)
  end
  sleep interval
end

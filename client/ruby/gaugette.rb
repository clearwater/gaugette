# This is the library class used by clients to talk to the arduino.
class Gaugette

  def initialize(device)
    @steps = 945
    @port = SerialPort.new(device, 9600)
  end

  def zero(address)
    @port.write("z %s 0\n" % [address])
  end

  def set(address, step)
    step = 0 if step<0
    #puts "s %s %d\n" % [address, step]
    @port.write "s %s %d\n" % [address, step]
  end

  def lcd(address, value)
    value = 0 if value<0
    value = 255 if value>255
    @port.write "l %s %d\n" % [address, value] 
  end

end

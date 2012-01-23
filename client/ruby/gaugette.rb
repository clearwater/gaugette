class Gaugette

  def initialize(device)
    @steps = 945
    @port = SerialPort.new(device, 9600)
  end

  def zero(address)
    @port.write("z %s 0\n" % [address])
  end

  def accel(address, accel, decel)
    @port.write("a %s %d,%d\n" % [address, accel, decel])
  end

  def delay(address, min, max)
    @port.write("d %s %d,%d\n" % [address, min, max])
  end

  def speed(address, min, max)
    @port.write("v %s %d,%d\n" % [address, min, max])
  end

  def set(address, fraction)
    @port.write "s %s %d\n" % [address, fraction * @steps]
  end

end

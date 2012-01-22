class Gaugette

  def initialize(device)
    @steps = 945
    @port = SerialPort.new(device, 9600)
  end

  def zero(address)
    @port.write("z %s 0\n" % [address])
  end

  def set(address, fraction)
    @port.write "s %s %d\n" % [address, fraction * @steps]
  end

end

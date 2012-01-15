class Gaugette

  

  def initialize(device)
    @address = 0
    @steps = 945
    @port = SerialPort.new(device, 9600)
    @port.write "z 0 0\n"
  end

  def write(fraction)
    @port.write "s 0 %d\n" % [fraction * @steps]
  end

end

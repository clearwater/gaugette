class Gaugette

  def initialize(device)
    @steps = 945
    @port = SerialPort.new(device, 9600)
  end

  def zero(address)
    @port.write("z %s 0\n" % [address])
  end

  def range(address, low, high)
    low=0 if low<0
    puts("r %s %d,%d\n" % [address, low, high])
    @port.write("r %s %d,%d\n" % [address, low, high])
  end

  def set(address, step)
    step = 0 if step<0
    puts "s %s %d\n" % [address, step]
    @port.write "s %s %d\n" % [address, step]
  end

end

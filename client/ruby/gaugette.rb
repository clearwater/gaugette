# This is the library class used by clients to talk to the arduino.
class Gaugette

  def initialize(device, steps=945)
    @default_steps = steps
    @port = SerialPort.new(device, 9600)
    @steps = {}
  end

  def write(*args)
    cmd = *args.join(' ')+"\n"
    # PP.pp cmd
    @port.write(cmd)
  end

  def zero(address)
    write('z',address,0)
  end

  def set(address, step)
    step = 0 if step<0
    write('s',address,step.to_i)
  end

  def range(address, steps)
    @steps[address] = steps
    write('r',address,steps.to_i)
  end

  def set_scaled(address, value, range)
    steps = @steps[address] || @default_steps
    set(address, value / range * steps)
  end
  
  def lcd(address, value)
    value = 0 if value<0
    value = 255 if value>255
    write('l',address,value.to_i)
  end

end

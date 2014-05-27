require "serialport"

# connect to bluetooth serial port
sp = SerialPort.new('/dev/rfcomm0', 38400, 8, 1, SerialPort::NONE)

# ensure it is synchronous
sp.sync = true

# delay to connect
sleep 1

# handshake
sp.write "Hello\r"
s = sp.gets.strip
if s != 'Matrix Clock'
  abort(Time.now.strftime('%F %T') + ' Handshake error!')
end

# get current time
sp.write("Time?\r");
puts Time.now.strftime('%F %T') + ' Current time: ' + sp.gets.strip

# set correct time
sp.write("Time=" + Time.now.strftime('%H%M%S') + "\r");
puts Time.now.strftime('%F %T') + ' Set time: ' + sp.gets.strip

# delay
sleep 1

# close connection
sp.close

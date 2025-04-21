clc
clear all
device1 = serialport("/dev/tty.usbserial-BG00QJ1K",115200);
device1.Timeout = 500;
flush(device1)
configureTerminator(device1,"CR/LF")
writeline(device1,"AT+PARAMETER=7,7,1,12")
ret = readline(device1)
while(1)
  ret = readline(device1) + " T = " + datestr(now)
end
% transmit = "AT+SEND=0,4,LEVI";
% writeline(device1,transmit);
% ret = readline(device1)
%start(device1,"continuous");
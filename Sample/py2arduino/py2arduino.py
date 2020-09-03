import serial
import time

ser =serial.Serial("COM4", 115200)
time.sleep(2)

# int型のみ
val = [180, 2000, 30]

s = str(val[0]) + '\1'
ser.write(s.encode())

s = str(val[1]) + '\2'
ser.write(s.encode())

s = str(val[2]) + '\3'
ser.write(s.encode())

ser.close()

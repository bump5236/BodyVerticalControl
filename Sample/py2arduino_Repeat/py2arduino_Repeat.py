import serial
import time


LOOP_TIME = 0.009
# int型のみ
val = [10, 100, 1000]
cnt = 0
timer = [0, 0, 0]

ser =serial.Serial("COM4", 57600)
time.sleep(2)

try:
    timer[0] = time.time()

    while True:
        print("LOOP TIME", time.time() - timer[1])

        timer[1] = time.time()
        val[0] = val[0] - cnt
        s = str(val[0]) + '\1'
        ser.write(s.encode())

        val[1] = val[1] - cnt
        s = str(val[1]) + '\2'
        ser.write(s.encode())

        val[2] = val[2] - cnt
        s = str(val[2]) + '\3'
        ser.write(s.encode())

        # val[2] = val[2] - cnt
        # s = str(val[2]) + '\4'
        # ser.write(s.encode())

        # val[2] = val[2] - cnt
        # s = str(val[2]) + '\5'
        # ser.write(s.encode())
        
        # val[2] = val[2] - cnt
        # s = str(val[2]) + '\6'
        # ser.write(s.encode())
        
        
        cnt = 1
        # if cnt > 120:
        #     val = [10, 100, 0]
        #     cnt = 0
        
        timer[2] = time.time() - timer[1]

        if timer[2] < LOOP_TIME:
            time.sleep(LOOP_TIME - timer[2])

except KeyboardInterrupt:
    ser.close()
    print("Fin...")

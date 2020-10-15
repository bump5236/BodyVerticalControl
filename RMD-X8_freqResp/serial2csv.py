import serial
import time
import csv
import os 
import re


def save(file_name, head, data):
    # normal save
    f = open(file_name,'w')
    csvWriter = csv.writer(f, lineterminator='\n')
    csvWriter.writerows(head)
    csvWriter.writerows(data)
    f.close()
    print("--- Saved ---")

def file_count():
    dir = os.getcwd()
    files = os.listdir(dir)
    count = 0
    for file in files:
        index = re.search('.csv', file)
        if index:
            count = count +1
    return count

class motor_data:
    def __init__(self):
        self.temperature = 0
        # self.add_cur = 0
        self.tgt_cur = 0
        self.current = 0
        self.velocity = 0
        self.angle = 0

m1 = motor_data()

ser =serial.Serial("/dev/ttyACM0", 115200)
time.sleep(2)

for _ in range(20):
    c = ser.readline()

org_data = []
save_data = []
val = []
timer = [0, 0, 0]

try:
    timer[0] = time.time()

    while True:
        timer[1] = time.time()
        c = ser.readline()
        c = c.strip().decode('utf-8')
        org_data.append(c.split(","))

        if len(org_data[-1]) == 6:
            val = org_data[-1]

            arduino_time = val[0]

            i_n = val[1]
            m1.tgt_cur = val[2]
            m1.current = val[3]
            m1.velocity = val[4]
            m1.angle = val[5]

            m1_values = [v for v in m1.__dict__.values()]

            l = [timer[1]-timer[0], arduino_time, i_n]
            l.extend(m1_values)
            save_data.append(l)

            print(save_data[-1])

except KeyboardInterrupt:
    ser.close()

    m1_keys = ["M1_"+ k for k in m1.__dict__.keys()]

    h = ["TIME", "ARDUINO_TIME", "i"]
    h.extend(m1_keys)

    header = [h]

    num = file_count()
    save("pos_kp120_ki40_kd50_" + str(num) + ".csv", header, save_data)
    print("--- Fin ---")

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
        self.add_cur = 0
        self.tgt_cur = 0
        self.current = 0
        self.velocity = 0
        self.angle = 0

m1 = motor_data()
m2 = motor_data()

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

        if len(org_data[-1]) == 14:
            val = org_data[-1]

            arduino_time = val[0]
            sync = val[1]

            m1.temperature = val[2]
            m1.add_cur = val[3]
            m1.tgt_cur = val[4]
            m1.current = val[5]
            m1.velocity = val[6]
            m1.angle = val[7]

            m2.temperature = val[8]
            m2.add_cur = val[9]
            m2.tgt_cur = val[10]
            m2.current = val[11]
            m2.velocity = val[12]
            m2.angle = val[13]

            m1_values = [v for v in m1.__dict__.values()]
            m2_values = [v for v in m2.__dict__.values()]

            l = [timer[1]-timer[0], arduino_time, sync]
            l.extend(m1_values)
            l.extend(m2_values)
            save_data.append(l)

            print(save_data[-1])

except KeyboardInterrupt:
    ser.close()

    m1_keys = ["M1_"+ k for k in m1.__dict__.keys()]
    m2_keys = ["M2_"+ k for k in m2.__dict__.keys()]

    h = ["TIME", "ARDUINO_TIME", "SYNC"]
    h.extend(m1_keys)
    h.extend(m2_keys)

    header = [h]

    num = file_count()
    save("output" + str(num) + ".csv", header, save_data)
    print("--- Fin ---")

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
        self.add_cur = 0
        self.tgt_cur = 0
        self.reply_buf1 = 0
        self.reply_buf2 = 0
        self.reply_buf3 = 0
        self.reply_buf4 = 0
        self.reply_buf5 = 0
        self.reply_buf6 = 0
        self.reply_buf7 = 0
        self.reply_buf8 = 0
        self.pos_buf1 = 0
        self.pos_buf2 = 0
        self.pos_buf3 = 0
        self.pos_buf4 = 0
        self.pos_buf5 = 0
        self.pos_buf6 = 0
        self.pos_buf7 = 0
        self.pos_buf8 = 0

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

        if len(org_data[-1]) == 20:
            val = org_data[-1]

            arduino_time = val[0]
            euler_z = val[1]

            m1.add_cur = val[2]
            m1.tgt_cur = val[3]
            m1.reply_buf1 = val[4]
            m1.reply_buf2 = val[5]
            m1.reply_buf3 = val[6]
            m1.reply_buf4 = val[7]
            m1.reply_buf5 = val[8]
            m1.reply_buf6 = val[9]
            m1.reply_buf7 = val[10]
            m1.reply_buf8 = val[11]
            m1.pos_buf1 = val[12]
            m1.pos_buf2 = val[13]
            m1.pos_buf3 = val[14]
            m1.pos_buf4 = val[15]
            m1.pos_buf5 = val[16]
            m1.pos_buf6 = val[17]
            m1.pos_buf7 = val[18]
            m1.pos_buf8 = val[19]

            m2.add_cur = val[20]
            m2.tgt_cur = val[21]
            m2.reply_buf1 = val[22]
            m2.reply_buf2 = val[23]
            m2.reply_buf3 = val[24]
            m2.reply_buf4 = val[25]
            m2.reply_buf5 = val[26]
            m2.reply_buf6 = val[27]
            m2.reply_buf7 = val[28]
            m2.reply_buf8 = val[29]
            m2.pos_buf1 = val[30]
            m2.pos_buf2 = val[31]
            m2.pos_buf3 = val[32]
            m2.pos_buf4 = val[33]
            m2.pos_buf5 = val[34]
            m2.pos_buf6 = val[35]
            m2.pos_buf7 = val[36]
            m2.pos_buf8 = val[37]

            m1_values = [v for v in m1.__dict__.values()]
            m2_values = [v for v in m2.__dict__.values()]

            l = [timer[1]-timer[0], arduino_time, euler_z]
            l.extend(m1_values)
            l.extend(m2_values)
            save_data.append(l)

            print(save_data[-1])

except KeyboardInterrupt:
    ser.close()

    m1_keys = ["M1_"+ k for k in m1.__dict__.keys()]
    m2_keys = ["M2_"+ k for k in m2.__dict__.keys()]

    h = ["TIME", "ARDUINO_TIME", "EULER_Z"]
    h.extend(m1_keys)
    h.extend(m2_keys)

    header = [h]

    num = file_count()
    save("output" + str(num) + ".csv", header, save_data)
    print("--- Fin ---")

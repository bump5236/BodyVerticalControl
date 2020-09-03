import serial
import time
import csv


def save(file_name, head, data):
    # normal save
    f = open(file_name,'w')
    csvWriter = csv.writer(f, lineterminator='\n')
    csvWriter.writerows(head)
    csvWriter.writerows(data)
    f.close()
    print("--- Saved ---")


ser =serial.Serial("COM9", 115200)
time.sleep(2)

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
            tgt_pos = val[1]
            reply_buf1 = val[2]
            reply_buf2 = val[3]
            reply_buf3 = val[4]
            reply_buf4 = val[5]
            reply_buf5 = val[6]
            reply_buf6 = val[7]
            reply_buf7 = val[8]
            reply_buf8 = val[9]
            pos_buf1 = val[10]
            pos_buf2 = val[11]
            pos_buf3 = val[12]
            pos_buf4 = val[13]
            pos_buf5 = val[14]
            pos_buf6 = val[15]
            pos_buf7 = val[16]
            pos_buf8 = val[17]

            save_data.append([
                timer[1]-timer[0],
                arduino_time,
                tgt_pos,
                reply_buf1,
                reply_buf2,
                reply_buf3,
                reply_buf4,
                reply_buf5,
                reply_buf6,
                reply_buf7,
                reply_buf8,
                pos_buf1,
                pos_buf2,
                pos_buf3,
                pos_buf4,
                pos_buf5,
                pos_buf6,
                pos_buf7,
                pos_buf8
            ])

        print(save_data[-1])

except KeyboardInterrupt:
    ser.close()
    
    header = [[
        "TIME",
        "ARDUINO_TIME",
        "TARGET",
        "RPBUF1",
        "RPBUF2",
        "RPBUF3",
        "RPBUF4",
        "RPBUF5",
        "RPBUF6",
        "RPBUF7",
        "RPBUF8",
        "POSBUF1",
        "POSBUF2",
        "POSBUF3",
        "POSBUF4",
        "POSBUF5",
        "POSBUF6",
        "POSBUF7",
        "POSBUF8"
    ]]

    save("output.csv", header, save_data)
    print("--- Fin ---")
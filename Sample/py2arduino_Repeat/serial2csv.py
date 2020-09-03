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
timer = [0, 0, 0]

try:
    timer[0] = time.time()

    while True:
        timer[1] = time.time()
        c = ser.readline()
        c = c.strip().decode('utf-8')
        org_data.append(c.split(","))
        if len(org_data[-1]) == 6:
            val1, val2, val3, val4, val5, val6 = [int(s) for s in org_data[-1]]
        else :
            val1, val2, val3, val4, val5, val6 = [0, 0, 0, 0, 0, 0]

        save_data.append([
            timer[1]-timer[0],
            val1, 
            val2, 
            val3, 
            val4, 
            val5, 
            val6
        ])

        print(save_data[-1])

except KeyboardInterrupt:
    ser.close()
    
    header = [[
        "TIME",
        "VAL1",
        "VAL2",
        "VAl3",
        "VAl4",
        "VAl5",
        "VAl6",
    ]]

    save("output.csv", header, save_data)
    print("--- Fin ---")
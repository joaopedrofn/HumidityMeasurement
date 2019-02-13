from serial import Serial
import time
import csv
import random
from datetime import datetime

# serial = Serial('COM14')

while True:
    time.sleep(5)
    with open('db.csv', 'a', newline='') as csvfile:
        data = []
        data.append(datetime.now())
        # serial.write(bytes('a', 'utf-8'))
        data.append(random.randint(0, 1000)/10)
        # data[0] = str(serial.read(4))
        # serial.write(bytes('s', 'utf-8'))
        # data[1] = str(serial.read(4))
        data.append(random.randint(0, 1023))
        csv.writer(csvfile).writerow(data)



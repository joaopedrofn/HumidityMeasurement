from serial import Serial
import time

serial = Serial('COM14')
while True:
    time.sleep(2)
    serial.write(bytes('a', 'utf-8'))
    print(str(serial.read(4)))
    serial.write(bytes('s', 'utf-8'))
    print(str(serial.read(4)))
# -*- coding: utf-8 -*-
from serial import Serial
import time
import csv
import matplotlib.pyplot as plt
import matplotlib.dates as dates
from datetime import datetime
from threading import Thread
import numpy as np

collect = False

def plot():
    with open('db.csv', 'rt') as csvfile:
        date =[]
        air = []
        soil = []
        raw_soil = []
        for row in csv.reader(csvfile):
            date.append(dates.date2num(datetime.strptime(row[0], '%Y-%m-%d %H:%M:%S.%f')))
            air.append(float(row[1])*0.1)
            soil.append(((float(row[2])-1023)/(-1023))*100)
            raw_soil.append(float(row[2]))
    fig1 = plt.figure()
    fig2 = plt.figure()
    pl1 = fig1.add_subplot(1,1,1)
    pl2 = fig2.add_subplot(1,1,1)
    pl22 = pl2.twinx()
    pl1.set_title('Umidade Relativa do Ar')
    pl2.set_title('Umidade do Solo')
    pl1.set_xlabel('Tempo')
    pl2.set_xlabel('Tempo')
    pl1.set_ylabel('Umidade %')
    pl2.set_ylabel('Umidade %')
    pl22.set_ylabel('Umidade sem tratamento')
    pl1.plot_date(date, air, 'b-')
    pl2.plot_date(date, soil, 'b-', color='g')
    pl22.plot_date(date, raw_soil, 'b-', color='g')
    pl22.invert_yaxis()
    plt.show()

def collectData():
    serial = Serial('COM7')
    global collect
    while collect:
    	time.sleep(600)
    	with open('db.csv', 'a', newline='') as csvfile:
            data = []
            data.append(datetime.now())
            serial.write(bytes('a', 'utf-8'))
            data.append(str(serial.read(4))[2:-5])
            serial.write(bytes('s', 'utf-8'))
            data.append(str(serial.read(4)).replace('\'', '').replace('b', '').replace('\\x00', ''))
            csv.writer(csvfile).writerow(data)


def listenKeyboard():
    op = 0
    while op != '1' and op != '2':
        print("Digite a opção desejada:\n")
        print("1. Coletar dados.\n")
        print("2. Plotar Gráficos.\n")
        op = input("Digite: ")
        if op == '1':
            global collect
            collect = True
            collectThread.start()
            q = ''
            while(q != 'q'):
                q = input("Para sair e plotar os gráficos digite 'q' e tecle 'enter'")
                if(q): 
                  collect = False
                  plot()
                  exit()
        elif op == '2':
            plot()
            exit()



collectThread = Thread(target=collectData)
listenThread = Thread(target=listenKeyboard)

listenThread.start()
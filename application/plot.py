import csv
import matplotlib.pyplot as plt

with open('db.csv', 'rt') as csvfile:
    date =[]
    air = []
    soil = []
    for row in csv.reader(csvfile):
        date.append(row[0])
        air.append(float(row[1]))
        soil.append(float(row[2]))
fig1 = plt.figure()
fig2 = plt.figure()
pl1 = fig1.add_subplot(1,1,1)
pl2 = fig2.add_subplot(1,1,1)
pl1.plot(date, air)
pl2.plot(date, soil)
plt.show()
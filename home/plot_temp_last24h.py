import pandas as pd
import numpy as np
import MySQLdb
import pandas_datareader.data as pdr
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime, timedelta
import requests
import os

dt_now = datetime.now()
# dt_now = dt_now + timedelta(1)
yesterday = dt_now - timedelta(1)

conn = MySQLdb.connect(user='user_homeHK', password='qAfbUr3rD7i44UihR', host='localhost', database='homeHK')
cur = conn.cursor()
cur.execute("select * from BME680_workroom where date_and_time >= DATE_SUB(NOW(), INTERVAL 24 HOUR);")

time_array_workroom  = []
temperature_array_workroom = []
pressure_local_array_workroom = []
pressure_sea_array_workroom = []
humidity_array_workroom = []



#終値がゼロ（取引がない）ときを除いて、データを配列に格納する。
for row in cur.fetchall():            
    time_array_workroom.append(row[0]) 
    temperature_array_workroom.append(row[1])
    pressure_local_array_workroom.append(row[2])
    pressure_sea_array_workroom.append(row[3])
    humidity_array_workroom.append(row[4])
    
    
cur.execute("select * from BME680_outside where date_and_time >= DATE_SUB(NOW(), INTERVAL 24 HOUR);")

time_array_outside  = []
temperature_array_outside = []
pressure_local_array_outside = []
pressure_sea_array_outside = []
humidity_array_outside = []



#終値がゼロ（取引がない）ときを除いて、データを配列に格納する。
for row in cur.fetchall():            
    time_array_outside.append(row[0]) 
    temperature_array_outside.append(row[1])
    pressure_local_array_outside.append(row[2])
    pressure_sea_array_outside.append(row[3])
    humidity_array_outside.append(row[4])
    
 
# plt.rcParams['font.family'] = 'Arial'
plt.rcParams["font.size"] = 28

fig, ax1 = plt.subplots(figsize=(20,13))
fig.autofmt_xdate()
formatter = mdates.DateFormatter("%H:%M")
ax1.xaxis.set_major_formatter(formatter)
span = pd.to_datetime([yesterday.strftime("%Y-%m-%d  %H:%M:%S"), dt_now.strftime("%Y-%m-%d  %H:%M:%S")])
ax1.scatter(time_array_workroom, temperature_array_workroom, s=60, c='k', marker='x', label='workroom')
ax1.scatter(time_array_outside, temperature_array_outside, s=60, c='r', marker='x', label='outside')
ax1.set_xlim(span)


ax1.set_title("Temperature history on " + yesterday.strftime("%Y-%m-%d") + " to " +  dt_now.strftime("%Y-%m-%d"))
ax1.set_xlabel("time")
ax1.set_ylabel("temperature, degC")
ax1.grid()
ax1.minorticks_on()
ax1.spines["top"].set_linewidth(7)
ax1.spines["left"].set_linewidth(7)
ax1.spines["bottom"].set_linewidth(7)
ax1.spines["right"].set_linewidth(7)
ax1.tick_params(direction="in", width=7, length=15)
ax1.tick_params(which='minor', direction="in", width=4, length=10)
ax1.legend()

plt.savefig("/var/www/html/home/temp/temperature_last24H_temp.jpg")

cur.close
conn.close
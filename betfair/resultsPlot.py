#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      antdi
#
# Created:     30/06/2020
# Copyright:   (c) antdi 2020
# Licence:     <your licence>
#-------------------------------------------------------------------------------


import matplotlib.pyplot as plt
import sys
import MySQLdb
import tkinter as tk
import datetime
from tkinter import simpledialog

def myprint(x):
    print(x)
    sys.stdout.flush()

#root = tk.Tk()
#root.geometry("500x100+800+800")
#root.withdraw()

try:
    numdays = int(input("Plot starting how many days ago? (0 = from start)"))
except:
    numdays = 0


# ignore the last couple of days
# as they tend to show 0 or low value which is subsequently updated
a = datetime.datetime.today()
enddatetime = a - datetime.timedelta(days = 2)
enddate = enddatetime.strftime("%Y-%m-%d")
startdatetime = a - datetime.timedelta(days = numdays)
startdate = startdatetime.strftime("%Y-%m-%d")
if (numdays == 0):
    startdate = "2020-11-04"
print (startdate)

indices = []
profits = []

title = "asdf"

# Open database connection
db = MySQLdb.connect(host='localhost',
								db='betfair',
								user='msgmedia',
								passwd='Media123!')

# prepare a cursor object using cursor() method
cursor = db.cursor()

index = 0 ;
profit = 0.0

sql = "select * from PROFIT ORDER BY DATE,TIME;"
try:
    # Execute the SQL command
    cursor.execute(sql)
    # Fetch all the rows in a list of lists.
    results = cursor.fetchall()
    for row in results:
        index = index + 1
        profit = float(row[14])
        #myprint("Index {} Profit {}".format(index, profit))
        profits.append(row[14])
        indices.append(index)
except:
   myprint ("Error: unable to fetch data")

# now plot it

# disconnect from server
db.close()

import matplotlib.pyplot as plt

plt.plot(indices, profits)

plt.show()


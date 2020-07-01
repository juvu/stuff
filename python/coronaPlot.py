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
import MySQLdb

town = input("Enter Town")

z = town.split('.')

print (z)

dates = []
cases = []
casesDiff = []
casesAve = []
newdates = []
newcasesDiff = []

title = "asdf"

# Open database connection
db = MySQLdb.connect(host='localhost',
								db='coronavirus',
								user='msgmedia',
								passwd='Media123!')

# prepare a cursor object using cursor() method
cursor = db.cursor()

if (len(z) > 1):
    sql = "SELECT DISTINCT * FROM CASES \
       WHERE AREA = '{0}' ORDER BY DATE".format(z[0])
    town = z[0]
else:
    sql = "SELECT DISTINCT * FROM CASES \
       WHERE AREA LIKE '%{0}%' ORDER BY DATE".format(town)

print (sql)
try:
    # Execute the SQL command
    cursor.execute(sql)
    # Fetch all the rows in a list of lists.
    results = cursor.fetchall()
    i=0
    lastcases=0
    for row in results:
        #t = row[1].strftime('%m/%d/%Y')
        title = "{0}".format(row[0])
        dates.append(row[1])
        cases.append(row[2])
        casesDiff.append(row[2] - lastcases)
        lastcases = row[2]
except:
   print ("Error: unable to fetch data")

from datetime import date, timedelta

# write out these results into the daily table

# Drop table if it already exist using execute() method.
cursor.execute("DROP TABLE IF EXISTS DAILY")

# Create table as per requirement
sql = """CREATE TABLE DAILY (
         AREA  CHAR(60) NOT NULL,
         DATE  DATE,
         DAILY INT)"""

cursor.execute(sql)

for i in range (len(casesDiff)):
    # Prepare SQL query to INSERT a record into the database.
    sql = "INSERT INTO DAILY(AREA, DATE, DAILY) \
            VALUES ('%s', '%s', '%s' )" % \
                (town, dates[i], casesDiff[i])
    print (sql)
    cursor.execute(sql)

#find missing dates
date_set = set(dates[0] + timedelta(x) for x in range((dates[-1] - dates[0]).days))
missing = sorted(date_set - set(dates))

#and insert these

for i in missing:
    # Prepare SQL query to INSERT a record into the database.
    sql = "INSERT INTO DAILY(AREA, DATE, DAILY) \
            VALUES ('%s', '%s', '0' )" % \
                (town, i)
    print (sql)
    cursor.execute(sql)

# Commit your changes in the database
db.commit()

# now read what we want from the new table

sql = "SELECT DISTINCT * FROM DAILY ORDER BY DATE;"

print (sql)
try:
    # Execute the SQL command
    cursor.execute(sql)
    # Fetch all the rows in a list of lists.
    results = cursor.fetchall()
    for row in results:
        newdates.append(row[1])
        newcasesDiff.append(row[2])
except:
   print ("Error: unable to fetch data")

# now plot it

print (dates)
print (newdates)
print (cases)
print (casesDiff)
print (newcasesDiff)

# now use the really complicated sql to get the 7 day average

sql = "select DISTINCT a.area,a.date,a.daily, \
        (SELECT SUM(b.daily) / COUNT(b.daily) from daily AS b \
        WHERE DATEDIFF(a.date, b.date) BETWEEN 0 and 6) \
        AS '7dayMovingAvg' FROM daily AS a WHERE \
        a.area like '%{0}%' ORDER BY a.date;".format(town)

try:
    # Execute the SQL command
    cursor.execute(sql)
    # Fetch all the rows in a list of lists.
    results = cursor.fetchall()
    for row in results:
        casesAve.append(row[3])
except:
   print ("Error: unable to fetch data")

# disconnect from server
db.close()

import matplotlib.dates as mdates

months = mdates.MonthLocator()  # every month
days = mdates.DayLocator()  # every day
months_fmt = mdates.DateFormatter('%B')


fig, (ax1, ax2) = plt.subplots(1, 2)

fig.suptitle(title)
ax1.plot(dates, cases)


# format the ticks
ax1.xaxis.set_major_locator(months)
ax1.xaxis.set_major_formatter(months_fmt)
#ax1.xaxis.set_minor_locator(days)
#ax1.grid(True)

ax2.plot(newdates, newcasesDiff)
ax2.plot (newdates, casesAve)


# format the ticks
ax2.xaxis.set_major_locator(months)
ax2.xaxis.set_major_formatter(months_fmt)
#ax2.xaxis.set_minor_locator(days)

# rotates and right aligns the x labels, and moves the bottom of the
# axes up to make room for them
#fig.autofmt_xdate()

plt.show()


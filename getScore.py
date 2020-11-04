import json
import sys
import datetime
import time
import urllib
import urllib.request
import urllib.error
import requests
import pandas as pd

import MySQLdb
import re
from typing import Iterable, Dict, Union, List

def connectDatabase():
    print("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost', db='test', user='msgmedia', passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    #cursor.execute("DROP TABLE IF EXISTS BACKSCORE")
    #db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE SCORES (
         MARKETID  CHAR(20) NOT NULL,
         SELECTIONID  CHAR(20) NOT NULL,
         BACK FLOAT,
         BACKSCORE FLOAT,
         FORECAST FLOAT,
         FORECASTSCORE FLOAT,
         PRIMARY KEY (MARKETID, SELECTIONID))"""

    #cursor.execute(sql)
    # Commit your changes in the database
    #db.commit()

    return (db, cursor)


def myprint(x):
    print(x)
    sys.stdout.flush()

db,cursor = connectDatabase()
sql = "SELECT * FROM BACK ORDER BY BACK"
cursor.execute(sql)
BackResults = cursor.fetchall()
sql = "SELECT * FROM FORECAST ORDER BY FORECAST"
cursor.execute(sql)
ForecastResults = cursor.fetchall()
sql = "SELECT * FROM FORM ORDER BY FORM"
cursor.execute(sql)
FormResults = cursor.fetchall()

print (BackResults)
print (ForecastResults)
print (FormResults)

back = float(input ("Enter back value to score"))
count = 0
lastback = 0.0
lastbackscore = 0.0
found = 0
for row in BackResults:
    if (back == row[0]):
        print ("Back Score is {}".format(row[1]))
        found = 1
        break
    elif (back > lastback and back < row[0]):
        score = (row[1] + lastbackscore) / 2.0
        print ("Back Score is {}".format(score))
        found = 1
        break
    lastback = row[0]
    lastbackscore = row[1]

if (found == 0):
    print ("Back Score is 1.0")

market = input ("Enter marketid")
sql = "SELECT MARKETID,SELECTIONID,RATING FROM RESULTS WHERE MARKETID = {} ORDER BY RATING DESC".format(market)
print (sql)
# Execute the SQL command
cursor.execute(sql)
# Fetch all the rows in a list of lists.
results = cursor.fetchall()
total = len(results)
rating = float(input ("Enter rating to score"))
count = 0.0
for row in results:
    if (rating > row[2]):
        count = count + 1.0
    elif (rating == row[2]):
        count = count + 0.5

ratingscore = float(count) / float(total)
print ("Rating Score is {}".format(ratingscore))


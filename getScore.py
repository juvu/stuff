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
sql = "SELECT MARKETID,SELECTIONID,BACK FROM RESULTS ORDER BY BACK"
print (sql)
# Execute the SQL command
cursor.execute(sql)
# Fetch all the rows in a list of lists.
results = cursor.fetchall()
total = len(results)
print (total)

back = float(input ("Enter back value to score"))
count = 0.0
for row in results:
    if (back > row[2]):
        count = count + 1.0
    elif (back == row[2]):
        count = count + 0.5

backscore = float(count) / float(total)
print ("Back Score is {}".format(backscore))

sql = "SELECT MARKETID,SELECTIONID,FORECAST FROM RESULTS ORDER BY FORECAST"
print (sql)
# Execute the SQL command
cursor.execute(sql)
# Fetch all the rows in a list of lists.
results = cursor.fetchall()
total = len(results)
forecast = float(input ("Enter forecast value to score"))
count = 0.0
for row in results:
    if (forecast > row[2]):
        count = count + 1.0
    elif (forecast == row[2]):
        count = count + 0.5

forecastscore = float(count) / float(total)
print ("Forecast Score is {}".format(forecastscore))


sql = "SELECT MARKETID,SELECTIONID,FORM FROM RESULTS ORDER BY FORM"
print (sql)
# Execute the SQL command
cursor.execute(sql)
# Fetch all the rows in a list of lists.
results = cursor.fetchall()
total = len(results)
form = input ("Enter form string to score")
count = 0.0
for row in results:
    if (form > row[2]):
        count = count + 1.0
    elif (form == row[2]):
        count = count + 0.5

formscore = float(count) / float(total)
print ("Form Score is {}".format(formscore))

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


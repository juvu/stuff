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

def myprint(x):
    print(x)
    sys.stdout.flush()

def connectDatabase():
    print("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost', db='test', user='msgmedia', passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    cursor.execute("DROP TABLE IF EXISTS PROFIT")
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE PROFIT (
         MARKETID  CHAR(20) NOT NULL,
         SELECTIONID  CHAR(20) NOT NULL,
         NAME CHAR (60),
         FORECAST FLOAT,
         FORM CHAR(20),
         RACE CHAR(60),
         DATE  DATE,
         TIME  TIME,
         VENUE CHAR(60),
         RATING INT,
         BACK FLOAT,
         LAY FLOAT,
         RESULT CHAR(20),
         SCORE FLOAT,
         PROFIT FLOAT,
         WINRATE FLOAT)"""


    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

db,cursor = connectDatabase()
sql = "SELECT * FROM SCORES"
cursor.execute(sql)
ScoreResults = cursor.fetchall()
profit = 0.0
winrate = 0.0
races = 0
for x in ScoreResults:
    score = float(x[13])
    lay = float(x[11])
    result = x[12]

    if (score > 1.55 and lay < 8.0):
        betsize = (2.0 * score)
        if (result == "LOSER"):
            profit = profit + betsize
        elif (result == "WINNER"):
            profit = profit - ((lay - 1.0) * betsize)
        else:
            continue

        races = races + 1
        winrate = profit / float(races)


        sql = "INSERT INTO PROFIT(MARKETID, SELECTIONID, FORECAST, BACK, LAY, DATE, TIME, VENUE, NAME, FORM, RACE, RATING, RESULT, SCORE, PROFIT, WINRATE) \
                VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s' , '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') " % \
                    (x[0], x[1], x[3], x[10], x[11], x[6], x[7], x[8], x[2], x[4], x[5], x[9], x[12], str(score), str(profit), str(winrate))
        #myprint (sql)
        cursor.execute(sql)

db.commit()



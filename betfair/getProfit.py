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
    db = MySQLdb.connect(host='localhost', db='betfair', user='msgmedia', passwd='Media123!')
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
         RUNNERS INT,
         WINRATE FLOAT)"""


    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

db,cursor = connectDatabase()
sql = "SELECT * FROM SCORES ORDER BY DATE,TIME"
cursor.execute(sql)
ScoreResults = cursor.fetchall()
profit = 0.0
totalbet = 0.0
winrate = 0.0
races = 0
for x in ScoreResults:
    score = float(x[13])
    runners = int(x[14])
    fav = x[15]
    rating = int(x[9])
    back = float(x[10])
    lay = float(x[11])
    forecast = float(x[3])
    result = x[12]
    race = x[5]
    form = x[4]

    #index = race.find("Hcap")
    #if (index < 0):
    #    continue

    
    #if (score > 1.52 and score < 2.1 and lay < 8.0) or (score > 1.41 and lay < forecast and lay < 8.0):
    #if (score > 1.52 and score < 2.1 and lay < 8.0):
    #if (score > 1.2 and lay < forecast and lay < 8.0):
    #if (score > 1.2 and lay < forecast and lay < 8.0 and form[-1] != '1'):
    #if (score > 1.2 and lay < forecast and lay < 8.0 and forecast > 8.0 and form[-1] != '2' and form[-1] != '1'):
    #if (score > 1.2 and lay < forecast and lay < 8.0 and forecast > 8.0 and rating != 1000 and forecast < 20.0 and form[-1] != '2' and form[-1] != '1'):
    #if (score > 1.2 and lay < forecast and lay < 8.0 and forecast > 8.0 and forecast < 50.0 and form[-1] != '2' and form[-1] != '1'):
    #if (score > 1.4 and score < 1.6 and form[-1] != '2' and form[-1] != '1'):
    #if (score > 1.2 and lay < forecast and lay < 7.5 and forecast > 7.5 and forecast < 20.0 and form[-1] != '2' and form[-1] != '1'):
    #if (forecast > lay and lay >= 8.0 and lay < 10.0 and form[-1] != '1' and rating != 1000):
    #if (score > 1.2 and lay < forecast and lay < 9.0 and forecast > 9.0):
    #if (lay <= 9.5 and forecast >= (lay + 4.0)):
    #if (fav == 'N' and lay < 9.5 and forecast >= lay and form[-1] != '-' and form[-1] != '/' and form[-1] != '1' and form[-1] != '2'):
    if (back > 20.7 and back < 26.7 and forecast > 15.0 and forecast < 82.9):
        #betsize = (2.0 * score)
        #betsize = 5.0
        #betsize = (score - 1.1) * 20.0
        #betsize = (score - 1.0) * 15.0 
        #betsize = (2.0 - score) * 10.0
        #betsize = (forecast - lay) + 2.0
        #betsize = (forecast - lay) 
        betsize = profit * 0.005
        if (betsize < 5.0):
            betsize =  5.0
        if (betsize > 20.0):
            betsize =  20.0
        if (result == "LOSER"):
            profit = profit + (betsize * 0.98)
        elif (result == "WINNER"):
            profit = profit - ((lay - 1.0) * betsize)
        else:
            continue

        totalbet = totalbet + betsize
        races = races + 1
        #winrate = profit / float(races)
        winrate = 1.0
        try:
            winrate = profit / totalbet
            winrate = int (winrate * 100.0)
        except:
            pass

        sql = "INSERT INTO PROFIT(MARKETID, SELECTIONID, FORECAST, BACK, LAY, DATE, TIME, VENUE, NAME, FORM, RACE, RATING, RESULT, SCORE, RUNNERS, PROFIT, WINRATE) \
                VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s' , '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') " % \
                    (x[0], x[1], x[3], x[10], x[11], x[6], x[7], x[8], x[2], x[4], x[5], x[9], x[12], str(score), str(runners), str(profit), str(winrate))
        #myprint (sql)
        cursor.execute(sql)

    if (back > 16.3 and back < 20.7 and forecast > 12.0 and forecast < 54.9):
        betsize = profit * 0.005
        if (betsize < 5.0):
            betsize =  5.0
        if (betsize > 20.0):
            betsize =  20.0
        if (result == "LOSER"):
            profit = profit - (betsize * 0.98)
        elif (result == "WINNER"):
            profit = profit + ((back - 1.0) * betsize)
        else:
            continue

        totalbet = totalbet + betsize
        races = races + 1
        #winrate = profit / float(races)
        winrate = 1.0
        try:
            winrate = profit / totalbet
            winrate = int (winrate * 100.0)
        except:
            pass

        sql = "INSERT INTO PROFIT(MARKETID, SELECTIONID, FORECAST, BACK, LAY, DATE, TIME, VENUE, NAME, FORM, RACE, RATING, RESULT, SCORE, RUNNERS, PROFIT, WINRATE) \
                VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s' , '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') " % \
                    (x[0], x[1], x[3], x[10], x[11], x[6], x[7], x[8], x[2], x[4], x[5], x[9], x[12], str(score), str(runners), str(profit), str(winrate))
        #myprint (sql)
        cursor.execute(sql)

db.commit()



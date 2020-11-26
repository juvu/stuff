import json
import sys
import datetime
import time
import urllib
import urllib.request
import urllib.error
import requests
import pandas as pd
import random

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
         WINRATE FLOAT)"""


    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

db,cursor = connectDatabase()
sql = "SELECT * FROM SCORES ORDER BY DATE,TIME"
cursor.execute(sql)
ScoreResults = cursor.fetchall()
NumResults = len(ScoreResults)
myprint ("Num results is {}".format(NumResults))


maxWinRate = 0.0
maxProfit = -10000.0
for y in range (500000):
    profit = 0.0
    totalbet = 0.0
    winrate = 0.0
    races = 0
    #myprint ("y is {}".format(y))

    back1 = float(random.randint(10, 300)) / 10.0
    back2 = float(random.randint(10, 300)) / 10.0
    if (back1 < back2):
        minback = back1
        maxback = back2
    else:
        minback = back2
        maxback = back1
    
    fave = int(random.randint(0, 1))

    runners1 = int(random.randint(1, 30))
    runners2 = int(random.randint(1, 30))
    if (runners1 < runners2):
        minrunners = runners1
        maxrunners = runners2
    else:
        minrunners = runners2
        maxrunners = runners1
    
    forecast1 = float(random.randint(10, 1000)) / 10.0
    forecast2 = float(random.randint(10, 1000)) / 10.0
    if (forecast1 < forecast2):
        minforecast = forecast1
        maxforecast = forecast2
    else:
        minforecast = forecast2
        maxforecast = forecast1
    
    for x in ScoreResults:
        score = float(x[13])
        runners = int(x[14])
        fav = 0
        if (x[15] == 'Y'):
            fav = 1
        rating = int(x[9])
        back = float(x[10])
        lay = float(x[11])
        forecast = float(x[3])
        result = x[12]
        race = x[5]
        form = x[4]
    
    
        if (back >= minback and back <= maxback and forecast >= minforecast and forecast <= maxforecast and runners >= minrunners and runners <= maxrunners and fav == fave):
            #betsize = (forecast - lay) + 2.0 
            betsize = 5.0
            if (betsize > 9.0):
                betsize = 9.0
            if (result == "LOSER"):
                profit = profit - betsize
            elif (result == "WINNER"):
                profit = profit + ((lay - 1.0) * betsize)
            else:
                continue
    
            totalbet = totalbet + betsize
            races = races + 1
            winrate = 1.0
            try:
                winrate = profit / totalbet
                winrate = int (winrate * 100.0)
            except:
                pass

    #if (profit > maxProfit and races > NumResults / 30.0):
    #if (winrate > maxWinRate and races > 30):
    if (profit > maxProfit):
        maxProfit = profit
        myprint ("MinBack {} maxBack {} minforecast {} maxforecast {} minrunners {} maxrunners{} fav {} profit {} races {} winrate {}".format (minback, maxback, minforecast, maxforecast, minrunners, maxrunners, fav, profit, races, winrate))

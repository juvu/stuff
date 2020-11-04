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
    cursor.execute("DROP TABLE IF EXISTS SCORES")
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE SCORES (
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
         PRIMARY KEY (MARKETID, SELECTIONID))"""


    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

db,cursor = connectDatabase()
sql = "SELECT * FROM BBACK ORDER BY BACK"
cursor.execute(sql)
BackResults = cursor.fetchall()
sql = "SELECT * FROM FFORECAST ORDER BY FORECAST"
cursor.execute(sql)
ForecastResults = cursor.fetchall()
sql = "SELECT * FROM FFORM ORDER BY FORM"
cursor.execute(sql)
FormResults = cursor.fetchall()

myprint (BackResults)
myprint (ForecastResults)
myprint (FormResults)

#sql = "SELECT * FROM RESULTS WHERE RESULT = 'WINNER' ORDER BY RATING DESC"
sql = "SELECT * FROM RESULTS ORDER BY RATING DESC"
#myprint (sql)
cursor.execute(sql)
results = cursor.fetchall()
total = len(results)

for x in results:
    back = x[10]
    count = 0
    lastback = 0.0
    lastbackscore = 0.0
    found = 0
    bscore = 1.0
    for row in BackResults:
        if (back == row[0]):
            bscore = row[1]
            break
        elif (back > lastback and back < row[0]):
            bscore = (row[1] + lastbackscore) / 2.0
            break
        lastback = row[0]
        lastbackscore = row[1]

    #myprint ("Back score is {}".format(bscore))

    forecast = x[3]
    count = 0
    lastforecast = 0.0
    lastforecastscore = 0.0
    found = 0
    forecastscore = 1.0
    for row in ForecastResults:
        if (forecast == row[0]):
            forecastscore = row[1]
            break
        elif (forecast > lastforecast and forecast < row[0]):
            forecastscore = (row[1] + lastforecastscore) / 2.0
            break
        lastforecast = row[0]
        lastforecastscore = row[1]

    #myprint ("forecast score is {}".format(forecastscore))

    form = x[4]

    formrev =  form[::-1]
    formrev = formrev.replace('0','9')
    formrev = formrev.replace('-','')
    formrev = formrev.replace('/','')
    form = formrev

    count = 0
    lastform = "" 
    lastformscore = 1.0
    found = 0
    formscore = 0.0
    for row in FormResults:
        if (form == row[0]):
            formscore = row[1]
            break
        elif (form > lastform and form < row[0]):
            formscore = (row[1] + lastformscore) / 2.0
            break
        lastform = row[0]
        lastformscore = row[1]

    #myprint ("form score is {}".format(formscore))

    sql = "SELECT MARKETID,SELECTIONID,RATING FROM RESULTS WHERE MARKETID = {} ORDER BY RATING DESC".format(x[0])
    #myprint (sql)
    cursor.execute(sql)
    RatingResults = cursor.fetchall()
    total = len(RatingResults)
    count = 0.0
    rating = x[9]
    for row in RatingResults:
        if (rating > row[2]):
            count = count + 1.0
        elif (rating == row[2]):
            count = count + 0.5

    ratingscore = float(count) / float(total)
    #myprint ("Rating score is {}".format(ratingscore))

    ratioscore = float(back) / float(forecast)
    #myprint ("Ratio score is {}".format(ratioscore))

    #score = bscore + ratingscore + forecastscore + formscore + ratioscore
    #score = ratingscore + formscore + ratioscore
    #score = ratioscore
    #score = bscore + ratingscore + forecastscore + formscore + ratioscore
    score = bscore + ratingscore + forecastscore + ratioscore
    if (x[12] == "WINNER"):
        myprint ("{} score is {} {} {} {} {} so {}".format(x[2], bscore, ratingscore, forecastscore, formscore, ratioscore, score))

    sql = "INSERT INTO SCORES(MARKETID, SELECTIONID, FORECAST, BACK, LAY, DATE, TIME, VENUE, NAME, FORM, RACE, RATING, RESULT, SCORE) \
                VALUES ('%s', '%s', '%s', '%s', '%s' , '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') " % \
                    (x[0], x[1], x[3], x[10], x[11], x[6], x[7], x[8], x[2], x[4], x[5], x[9], x[12], str(score))
    cursor.execute(sql)

db.commit()



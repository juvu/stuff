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
    myprint("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost', db='test', user='msgmedia', passwd='Media123!')
    myprint("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    cursor.execute("DROP TABLE IF EXISTS BBACK")
    cursor.execute("DROP TABLE IF EXISTS FFORECAST")
    cursor.execute("DROP TABLE IF EXISTS FFORM")
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE BBACK (
         BACK FLOAT,
         SCORE FLOAT) """

    cursor.execute(sql)
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE FFORECAST (
         FORECAST FLOAT,
         SCORE FLOAT) """

    cursor.execute(sql)
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE FFORM (
         FORM CHAR(40),
         SCORE FLOAT) """

    cursor.execute(sql)
    db.commit()

    return (db, cursor)


db,cursor = connectDatabase()
sql = "SELECT DISTINCT BACK FROM RESULTS ORDER BY BACK"
myprint (sql)
# Execute the SQL command
cursor.execute(sql)
# Fetch all the rows in a list of lists.
results = cursor.fetchall()
total = len(results)

count = 0.0
for row in results:
    score = float (count) / float(total)        
    count = count + 1.0

    #myprint ("Back Score is {}".format(score))
    sql = "INSERT INTO BBACK(BACK, SCORE) VALUES ('%s', '%s') " % \
                    (str(row[0]), str(score))
    cursor.execute(sql)

db.commit()


sql = "SELECT DISTINCT FORECAST FROM RESULTS ORDER BY FORECAST"
cursor.execute(sql)
results = cursor.fetchall()
total = len(results)

count = 0.0
for row in results:
    score = float (count) / float(total)        
    count = count + 1.0

    #myprint ("Forecast Score is {}".format(score))
    sql = "INSERT INTO FFORECAST(FORECAST, SCORE) VALUES ('%s', '%s') " % \
                    (str(row[0]), str(score))
    cursor.execute(sql)

db.commit()

sql = "SELECT DISTINCT FORM FROM RESULTS ORDER BY FORM"
cursor.execute(sql)
results = cursor.fetchall()
total = len(results)
for row in results:
    runnerform =  row[0]
    runnerformrev =  runnerform[::-1]
    runnerformrev = runnerformrev.replace('0','9')
    runnerformrev = runnerformrev.replace('-','')
    runnerformrev = runnerformrev.replace('/','')
    sql = "INSERT INTO FFORM(FORM, SCORE) VALUES ('%s', '0.0') " % \
                    (str(runnerformrev))
    cursor.execute(sql)

db.commit()

sql = "SELECT FORM FROM FFORM ORDER BY FORM"
cursor.execute(sql)
results = cursor.fetchall()
total = len(results)
count = 0.0
for row in results:
    score = float (count) / float(total)        
    count = count + 1.0

    #myprint ("Form Score is {}".format(score))
    sql = "UPDATE FFORM SET SCORE = '{}' WHERE FORM = '{}'" .format(str(score), str(row[0]))
    cursor.execute(sql)

db.commit()

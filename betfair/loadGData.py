import json
import sys
import datetime
import time
import os
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
    db = MySQLdb.connect(host='localhost', db='betfair', user='msgmedia', passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    cursor.execute("DROP TABLE IF EXISTS GREYHOUNDS")
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE GREYHOUNDS (
         EVENTID  CHAR(20) NOT NULL,
         VENUE  CHAR(60) NOT NULL,
         EVENTNAME  CHAR(60) NOT NULL,
         NAME  CHAR(60) NOT NULL,
         DATE  DATE,
         TIME  TIME,
         BACK FLOAT,
         RESULT CHAR(20),
         PRIMARY KEY (EVENTID, NAME))"""

    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

def getGData(db, cursor):
    #f = open("combined", "r")
    for jsonResponse in sys.stdin: 
    #for jsonResponse in f: 
        #print (jsonResponse)
        try:
            result = json.loads(jsonResponse)
            country = result['mc'][0]['marketDefinition']['countryCode']
            eventName = result['mc'][0]['marketDefinition']['name']
            venue = result['mc'][0]['marketDefinition']['venue']
            eventId = result['mc'][0]['id']
            openDate = result['mc'][0]['marketDefinition']['openDate']
            my_datetime = datetime.datetime.strptime(openDate, '%Y-%m-%dT%H:%M:%S.000Z')
            StartTime = my_datetime.strftime('%H:%M')
            StartDate = my_datetime.strftime('%Y-%m-%d')
            if (country == "GB" and eventName != "To Be Placed"):
                for x in result['mc'][0]['marketDefinition']['runners']:
                    status = x['status']
                    if (status == "WINNER"):
                        sql = "INSERT INTO GREYHOUNDS (EVENTID, VENUE, EVENTNAME, NAME, BACK, DATE, TIME, RESULT) \
                                VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', 'WINNER') ON DUPLICATE KEY UPDATE BACK = '%s'" % \
                                (eventId, venue, eventName, x['name'], x['bsp'], StartDate, StartTime, x['bsp'])
                        cursor.execute(sql)
                        print ("{} {} {} {} {} WINNER".format(StartDate, StartTime, eventId, x['name'],x['bsp']))
                    elif (status == "LOSER"):
                        sql = "INSERT INTO GREYHOUNDS (EVENTID, VENUE, EVENTNAME, NAME, BACK, DATE, TIME, RESULT) \
                                VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', 'LOSER') ON DUPLICATE KEY UPDATE BACK = '%s'" % \
                                (eventId, venue, eventName, x['name'], x['bsp'], StartDate, StartTime, x['bsp'])
                        cursor.execute(sql)
                        print ("{} {} {} {} {} LOSER".format(StartDate, StartTime, eventId, x['name'],x['bsp']))
        except:
            pass

db,cursor = connectDatabase()

getGData(db, cursor)
db.commit()








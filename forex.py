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


import MySQLdb
import re
from typing import Iterable, Dict, Union, List
from json import dumps
from requests import get
from http import HTTPStatus

def connectDatabase():
    print("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost',
								db='finance',
								user='msgmedia',
								passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    cursor.execute("DROP TABLE IF EXISTS FOREX")
    db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE FOREX (
         ROWC INT,
         SYMBOL  CHAR(10) ,
         PERIOD  CHAR(10) ,
         DATE DATETIME,
         OPEN FLOAT,
         HIGH FLOAT,
         LOW FLOAT,
         CLOSE FLOAT,
         VOLUME INT,
         PRIMARY KEY (ROWC))"""

    cursor.execute(sql)
    # Commit your changes in the database
    db.commit()

    return (db, cursor)

def loadFileData(db, cursor):
    fx = input("Enter FX combo")
    period = input("Enter period")
    filepath = "C:/Users/antdi/Downloads/{}{}.csv".format(fx,period)
    nextRow = 1
    fp = open(filepath)
    for line in fp:
        w = line.rstrip()
        z = w.split('\t')
        print (z)

        # Prepare SQL query to INSERT a record into the database.
        sql = "INSERT INTO FOREX(ROWC, SYMBOL, PERIOD, DATE, OPEN, HIGH, LOW, CLOSE, VOLUME) \
                VALUES ('%s', '%s', '%s', '%s:00', '%s', '%s', '%s', '%s', '%s')" % \
                    (nextRow, fx, period, z[0], z[1], z[2], z[3], z[4], z[5])
        print (sql)
        nextRow = nextRow + 1
        try:
            # Execute the SQL command
            cursor.execute(sql)


        except MySQLdb.Error as e:
            print (e)
            return

    # Commit your changes in the database
    db.commit()

import os
import subprocess

from datetime import date
today = str(date.today())

db,cursor = connectDatabase()
loadFileData (db, cursor)

# disconnect from server

db.close()

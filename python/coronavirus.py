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

print("Connecting to database using MySQLdb")
db = MySQLdb.connect(host='localhost',
								db='coronavirus',
								user='msgmedia',
								passwd='Media123!')
print("Succesfully Connected to database using MySQLdb!")

# prepare a cursor object using cursor() method
cursor = db.cursor()

# Drop table if it already exist using execute() method.
cursor.execute("DROP TABLE IF EXISTS CASES")

# Create table as per requirement
sql = """CREATE TABLE CASES (
         AREA  CHAR(60) NOT NULL,
         DATE  DATE,
         CASES INT,
         RATE FLOAT )"""

cursor.execute(sql)
# Commit your changes in the database
db.commit()


filepath = "C:/Users/antdi/Downloads/coronavirus-csv.csv"
first = 1
fp = open(filepath)
for line in fp:
    w = line.rstrip()
    x = re.sub(r', ', ' ', w)
    y = re.sub(r'"', "", x)
    z = y.split(',')
    if (first == 0):
        # Prepare SQL query to INSERT a record into the database.
        sql = "INSERT INTO CASES(AREA, DATE, CASES, RATE) \
            VALUES ('%s', '%s', '%s', '%s' )" % \
                (z[0], z[3], z[7], z[10])
        print (sql)
        try:
            # Execute the SQL command
            cursor.execute(sql)

        except:
            # Rollback in case there is any error
            print ("Error....")
            #db.rollback()

    first = 0

# Commit your changes in the database
db.commit()

# and also the global data

filepath = "C:/Users/antdi/Downloads/full_data.csv"
first = 1
fp = open(filepath)
for line in fp:
    w = line.rstrip()
    x = re.sub(r', ', ' ', w)
    y = re.sub(r'"', "", x)
    z = y.split(',')
    if (first == 0):
        # Prepare SQL query to INSERT a record into the database.
        sql = "INSERT INTO CASES(AREA, DATE, CASES, RATE) \
            VALUES ('%s', '%s', '%s', '0.0' )" % \
                (z[1], z[0], z[4])
        print (sql)
        try:
            # Execute the SQL command
            cursor.execute(sql)

        except:
            # Rollback in case there is any error
            print ("Error....")
            #db.rollback()

    first = 0

# Commit your changes in the database
db.commit()

import datetime

# and the Welsh data
filepath = "C:/Users/antdi/Downloads/Wales-csv.csv"
first = 1
fp = open(filepath)
for line in fp:
    w = line.rstrip()
    x = re.sub(r', ', ' ', w)
    y = re.sub(r'"', "", x)
    z = y.split(',')
    zz = str(z[1])
    if (first == 0):
        format_str = '%d/%m/%Y' # The format
        zzz = datetime.datetime.strptime(zz, format_str)
        # Prepare SQL query to INSERT a record into the database.
        sql = "INSERT INTO CASES(AREA, DATE, CASES, RATE) \
            VALUES ('%s', '%s', '%s', '0.0' )" % \
                (z[0], zzz, z[3])
        print (sql)
        try:
            # Execute the SQL command
            cursor.execute(sql)

        except:
            # Rollback in case there is any error
            print ("Error....")
            #db.rollback()

    first = 0

# Commit your changes in the database
db.commit()
# disconnect from server
db.close()
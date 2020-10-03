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
								db='coronavirus',
								user='msgmedia',
								passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    #cursor.execute("DROP TABLE IF EXISTS CASES")
    #db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE CASES (
         AREA  CHAR(60) NOT NULL,
         DATE  DATE,
         CASES INT,
         RATE FLOAT,
         PRIMARY KEY (AREA, DATE))"""

    #cursor.execute(sql)
    # Commit your changes in the database
    #db.commit()

    return (db, cursor)

def UKCoronavirusCSV(db, cursor):
    index = input("Enter Uk coronavirus file index")
    filepath = "C:/Users/antdi/Downloads/coronavirus-cases_latest ({}).csv".format(index)
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

StructureType = Dict[str, Union[dict, str]]
FiltersType = Iterable[str]
APIResponseType = Union[List[StructureType], str]

def get_paginated_dataset(filters: FiltersType, structure: StructureType,
                          as_csv: bool = False) -> APIResponseType:
    """
    Extracts paginated data by requesting all of the pages
    and combining the results.

    Parameters
    ----------
    filters: Iterable[str]
        API filters. See the API documentations for additional
        information.

    structure: Dict[str, Union[dict, str]]
        Structure parameter. See the API documentations for
        additional information.

    as_csv: bool
        Return the data as CSV. [default: ``False``]

    Returns
    -------
    Union[List[StructureType], str]
        Comprehensive list of dictionaries containing all the data for
        the given ``filters`` and ``structure``.
    """
    endpoint = "https://api.coronavirus.data.gov.uk/v1/data"

    api_params = {
        "filters": str.join(";", filters),
        "structure": dumps(structure, separators=(",", ":")),
        "format": "json" if not as_csv else "csv"
    }

    data = list()

    page_number = 1

    while True:
        # Adding page number to query params
        api_params["page"] = page_number
        print (page_number)

        response = get(endpoint, params=api_params, timeout=10)

        if response.status_code >= HTTPStatus.BAD_REQUEST:
            raise RuntimeError(f'Request failed: {response.text}')
        elif response.status_code == HTTPStatus.NO_CONTENT:
            break

        if as_csv:
            csv_content = response.content.decode()

            # Removing CSV header (column names) where page
            # number is greater than 1.
            if page_number > 1:
                data_lines = csv_content.split("\n")[1:]
                csv_content = str.join("\n", data_lines)

            data.append(csv_content.strip())
            page_number += 1
            continue

        current_data = response.json()
        page_data: List[StructureType] = current_data['data']
        data.extend(page_data)

        # The "next" attribute in "pagination" will be `None`
        # when we reach the end.
        if current_data["pagination"]["next"] is None:
            break

        page_number += 1

    if not as_csv:
        return data

    # Concatenating CSV pages
    return str.join("\n", data)

def UKCoronavirusAPI(db, cursor, query_filters):

    query_structure = {
        "date": "date",
        "name": "areaName",
        "cumulative": "cumCasesBySpecimenDate"
    }

    json_data = get_paginated_dataset(query_filters, query_structure)

    for x in json_data:
        # Prepare SQL query to INSERT a record into the database.
        sql = "INSERT INTO CASES(AREA, DATE, CASES, RATE) \
                VALUES ('%s', '%s', '%s', '%s' ) ON DUPLICATE KEY UPDATE CASES = '%s'" % \
                    (x['name'], x['date'], x['cumulative'], '0.0', x['cumulative'])
        print (sql)
        try:
            # Execute the SQL command
            cursor.execute(sql)

        except:
            # Rollback in case there is any error
            print ("Error....")
            #db.rollback()

    # Commit your changes in the database
    db.commit()


def GlobalCoronavirusCSV(db, cursor):
    index = input("Enter world file index")
    filepath = "C:/Users/antdi/Downloads/owid-covid-data ({}).csv".format(index)
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
                    (z[2], z[3], z[4])
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

def GlobalCoronavirusAPI(db, cursor):

    endpoint = "https://api.covid19api.com/countries"
    response = get(endpoint)
    json_data = response.json()
    Slug = []

#    country_data: List[StructureType] = json_data['Countries']

    for x in json_data:
        Slug.append(x['Slug'])

    for z in Slug:
        endpoint = "https://api.covid19api.com/dayone/country/{}".format(z)
        try:
            response = get(endpoint,timeout=1)
        except:
            asdf = "Could not get {} - moving on".format(z)
            print(asdf)
            continue

        json_data = response.json()

        for y in json_data:
            # Prepare SQL query to INSERT a record into the database.
            country = "{}".format(y['Country'])
            dateStr = y['Date']
            shortDate = dateStr[0:10]
            sql = "INSERT INTO CASES(AREA, DATE, CASES, RATE) \
                VALUES ('%s', '%s', '%s', '%s' ) ON DUPLICATE KEY UPDATE CASES = '%s'" % \
                    (country, shortDate, y['Confirmed'], '0.0', y['Confirmed'])
            print (sql)
            try:
                # Execute the SQL command
                cursor.execute(sql)

            except:
                # Rollback in case there is any error
                print ("Error....")
                #db.rollback()

        # Commit your changes in the database
        db.commit()

import datetime

def welshCSV():
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

import os
import subprocess

from datetime import date
today = str(date.today())


file = "corona.sql.{}".format(today)
myoutput = open(file,'w')
#cp = subprocess.run (["C:\Program Files\MySQL\MySQL Utilities 1.6\mysqldbexport", "--server=root:Media123!@localhost:3306", "--export=DATA", "--all"], stdout=myoutput, stderr=subprocess.PIPE)
#print (cp.stderr)
#print (cp.stdout)


db,cursor = connectDatabase()

# UK data from CSV
#UKCoronavirusCSV(db, cursor)
# UK data from the API
#query_filters = [
#    f"areaType=LTLA"
#]


query_filters = [
    f"areaType=overview"
]
UKCoronavirusAPI(db, cursor, query_filters)
query_filters = [
    f"areaType=nation"
]
UKCoronavirusAPI(db, cursor, query_filters)
query_filters = [
    f"areaType=region"
]

a = datetime.datetime.today()
numdays = int(input("Enter number of days data to collect"))
dateList = []
for x in range (0, numdays):
    newdate = a - datetime.timedelta(days = x)
    dateList.append(newdate.strftime("%Y-%m-%d"))
print (dateList)

for x in dateList:
    query_filters = [
        "areaType=utla;date={}".format(x)
    ]
    UKCoronavirusAPI(db, cursor, query_filters)

for x in dateList:
    query_filters = [
        "areaType=ltla;date={}".format(x)
    ]
    UKCoronavirusAPI(db, cursor, query_filters)


# Global data from CSV
# GlobalCoronavirusCSV(db, cursor)
# Global data from the API
#GlobalCoronavirusAPI(db, cursor)

# Welsh data
# welshCSV()

# disconnect from server

db.close()
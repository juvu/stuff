#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy
import pandas
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
# Scikit's LinearRegression model
from sklearn.linear_model import LinearRegression
from scipy import stats
import math
import os
import requests
import json
import time
import random
import uuid
from time import localtime, strftime
import datetime
import calendar
import sys
import operator
import sched
from random import shuffle
import holidays
import pytz


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
    #cursor.execute("DROP TABLE IF EXISTS FOREX")
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

    #cursor.execute(sql)
    # Commit your changes in the database
    #db.commit()

    return (db, cursor)


def getPrices (epic_id, row, period, num):

    open_prices = []
    high_prices = []
    low_prices = []
    mid_prices = []
    close_prices = []

    sql = "SELECT * from FOREX WHERE ROWC > {} order by rowc limit {}".format(row,num)
    cursor.execute(sql)
    results = cursor.fetchall()
    for x in results:
        openval = int(float(x[4]) * 10000.0)
        highval = int(float(x[5]) * 10000.0)
        lowval = int(float(x[6]) * 10000.0)
        closeval = int(float(x[7]) * 10000.0)
        midval = midpoint (lowval, highval)
        open_prices.append(openval)
        high_prices.append(highval)
        low_prices.append(lowval)
        close_prices.append(closeval)
        mid_prices.append(midval)

    return  open_prices, high_prices, low_prices, mid_prices, close_prices


def midpoint(p1, p2):
    return (p1 + p2) / 2


if __name__ == '__main__':

    runningTotal = 0
    numTrades = 0
    numSuccess = 0
    numFailure = 0
    numUnknown = 0
    numOutstanding = 0
    numFailureCandle = 0
    numSuccessCandle = 0
    db,cursor = connectDatabase()
    for goes in range(3000):
        tradeable_epic_ids = []
        tradeable_epic_ids.append("EURUSD")
        for epic_id in tradeable_epic_ids:

            #theRow = random.randint (0,49800)
            theRow = goes
            open_prices, high_prices, low_prices, mid_prices, close_prices = getPrices(epic_id, theRow, 5, 16) ;
            copen_prices, chigh_prices, clow_prices, cmid_prices, cclose_prices = getPrices(epic_id, theRow, 5, 1000) ;
            current_bid = cmid_prices[16]
            #print ("Current bid {}".format(current_bid))
            #print (mid_prices)
            #print (copen_prices)

            open_prices = numpy.asarray(open_prices)
            close_prices = numpy.asarray(close_prices)
            low_prices = numpy.asarray(low_prices)
            high_prices = numpy.asarray(high_prices)
            mid_prices = numpy.asarray(mid_prices)

            xi = numpy.arange(0, len(mid_prices))

            close_prices_slope, close_prices_intercept, close_prices_r_value, close_prices_p_value, close_prices_std_err = stats.linregress(xi, close_prices)
            low_prices_slope, low_prices_intercept, low_prices_r_value, low_prices_p_value, low_prices_std_err = stats.linregress(xi, low_prices)
            high_prices_slope, high_prices_intercept, high_prices_r_value, high_prices_p_value, high_prices_std_err = stats.linregress(xi, high_prices)
            mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err = stats.linregress(xi, mid_prices)
            open_prices_slope, open_prices_intercept, open_prices_r_value, open_prices_p_value, open_prices_std_err = stats.linregress(xi, open_prices)

            #print ("epic {} close_prices_slope {} close_prices_intercept {} close_prices_r_value {} close_prices_p_value {} close_prices_std_err {}".format(
            #    epic_id, close_prices_slope, close_prices_intercept, close_prices_r_value, close_prices_p_value, close_prices_std_err))
            #print ("epic {} low_prices_slope {} low_prices_intercept {} low_prices_r_value {} low_prices_p_value {} low_prices_std_err {}".format(
            #    epic_id, low_prices_slope, low_prices_intercept, low_prices_r_value, low_prices_p_value, low_prices_std_err))
            #print ("epic {} high_prices_slope {} high_prices_intercept {} high_prices_r_value {} high_prices_p_value {} high_prices_std_err {}".format(
            #    epic_id, high_prices_slope, high_prices_intercept, high_prices_r_value, high_prices_p_value, high_prices_std_err))


            cbid = float(current_bid)
            midIntercept = float (mid_prices_intercept)
            midSlope = float (mid_prices_slope)

            midStopAt = midIntercept +  ((len(mid_prices) - 12) * midSlope)
            midLatest = midIntercept +  (len(mid_prices) * midSlope)

            TRADE_DIRECTION = "NONE"
            if (midSlope < -1.0):
                stopDist = int(midIntercept - cbid)
                stopDist = int(midStopAt - cbid)
                pip_limit = int(midLatest - cbid)
                #print ("midLatest {} stopDist {} pip_limit {}".format(midLatest, stopDist, pip_limit))
                if (pip_limit >= 4 and stopDist >= 10):
                    TRADE_DIRECTION = "SELL"
                    numTrades = numTrades + 1

            elif (midSlope > 1.0):
                stopDist = int(cbid - midIntercept)
                stopDist = int(cbid - midStopAt)
                pip_limit = int(cbid - midLatest)
                #print ("midLatest {} stopDist {} pip_limit {}".format(midLatest, stopDist, pip_limit))
                if (pip_limit >= 4 and stopDist >= 10):
                    TRADE_DIRECTION = "BUY"
                    numTrades = numTrades + 1

            if (TRADE_DIRECTION == "NONE"):
                asdf = "asdf"
                #print ("No Trade")
            elif (TRADE_DIRECTION == "BUY"):
                for i in range (len(copen_prices)):
                    if (i < 16):
                        continue
                    # this is the auto reverse stuff
                    nopen_prices, nhigh_prices, nlow_prices, nmid_prices, nclose_prices = getPrices(epic_id, i+theRow-16, 5, 16) ;
                    nmidPrice = int(cmid_prices[i])
                    nmid_prices = numpy.asarray(nmid_prices)
                    xi = numpy.arange(0, len(nmid_prices))
                    nmid_prices_slope, nmid_prices_intercept, nmid_prices_r_value, nmid_prices_p_value, nmid_prices_std_err = stats.linregress(xi, nmid_prices)
                    nmidIntercept = float (nmid_prices_intercept)
                    nmidSlope = float (nmid_prices_slope)

                    if (nmidSlope < -1.0):
                        runningTotal = runningTotal - (cbid - nmidPrice)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure (GONE BAD) on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] >= (cbid + pip_limit) and clow_prices[i] > (cbid - stopDist)):
                        runningTotal = runningTotal + pip_limit
                        numSuccess = numSuccess + 1
                        numSuccessCandle = numSuccessCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade success on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] < (cbid + pip_limit) and clow_prices[i] < (cbid - stopDist)):
                        runningTotal = runningTotal - stopDist
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] >= (cbid + pip_limit) and clow_prices[i] < (cbid - stopDist)):
                        numUnknown = numUnknown + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade unknown on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (i == len(copen_prices) - 1):
                        numOutstanding = numOutstanding + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade still open on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
            elif (TRADE_DIRECTION == "SELL"):
                for i in range (len(copen_prices)):
                    if (i < 16):
                        continue
                    # this is the auto reverse stuff
                    nopen_prices, nhigh_prices, nlow_prices, nmid_prices, nclose_prices = getPrices(epic_id, i+theRow-16, 5, 16) ;
                    nmidPrice = int(cmid_prices[i])
                    nmid_prices = numpy.asarray(nmid_prices)
                    xi = numpy.arange(0, len(nmid_prices))
                    nmid_prices_slope, nmid_prices_intercept, nmid_prices_r_value, nmid_prices_p_value, nmid_prices_std_err = stats.linregress(xi, nmid_prices)
                    nmidIntercept = float (nmid_prices_intercept)
                    nmidSlope = float (nmid_prices_slope)

                    if (nmidSlope > 1.0):
                        runningTotal = runningTotal - (cbid - nmidPrice)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i - 16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure (GONE BAD) on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] <= (cbid - pip_limit) and chigh_prices[i] < (cbid + stopDist)):
                        runningTotal = runningTotal + pip_limit
                        numSuccess = numSuccess + 1
                        numSuccessCandle = numSuccessCandle + i - 16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade success on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] > (cbid - pip_limit) and chigh_prices[i] > (cbid + stopDist)):
                        runningTotal = runningTotal - stopDist
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] <= (cbid - pip_limit) and chigh_prices[i] > (cbid + stopDist)):
                        numUnknown = numUnknown + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade unknown on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (i == len(copen_prices) - 1):
                        numOutstanding = numOutstanding + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade still open on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))

    db.close()
    try:
        print ("average success candle {} average failure candle {}".format(numSuccessCandle / numSuccess,numFailureCandle /numFailure))
    except:
        pass


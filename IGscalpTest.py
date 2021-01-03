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

SL_MULTIPLIER = 4
LOW_SL_WATERMARK = 10
HIGH_SL_WATERMARK = 90
STR_CHECK = ""

def distance(a, b):
    if (a == b):
        return 0
    elif (a < 0) and (b < 0) or (a > 0) and (b > 0):
        if (a < b):
            return (abs(abs(a) - abs(b)))
        else:
            return -(abs(abs(a) - abs(b)))
    else:
        return math.copysign((abs(a) + abs(b)), b)


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

def Chandelier_Exit_formula(TRADE_DIR, ATR, Price):

    if TRADE_DIR == "BUY":

        return float(Price) - float(ATR) * 3

    elif TRADE_DIR == "SELL":

        return float(Price) + float(ATR) * 3

def calculate_stop_loss(close_prices,high_prices,low_prices):

    price_ranges = []
    closing_prices = []
    first_time_round_loop = True
    TR_prices = []
    price_compare = "bid"

    for i in range (16):
        if first_time_round_loop:
            # First time round loop cannot get previous
            closePrice = close_prices[i]
            closing_prices.append(closePrice)
            high_price = high_prices[i]
            low_price = low_prices[i]
            price_range = float(high_price - closePrice)
            price_ranges.append(price_range)
            first_time_round_loop = False
        else:
            prev_close = closing_prices[-1]
            ###############################
            closePrice = close_prices[i]
            closing_prices.append(closePrice)
            high_price = high_prices[i]
            low_price = low_prices[i]
            price_range = float(high_price - closePrice)
            price_ranges.append(price_range)
            TR = max(high_price - low_price,
                     abs(high_price - prev_close),
                     abs(low_price - prev_close))
            TR_prices.append(TR)

    return str(int(float(max(TR_prices))))

def midpoint(p1, p2):
    return (p1 + p2) / 2


if __name__ == '__main__':

    runningTotal = 0
    numTrades = 0
    numSuccess = 0
    numFailure = 0
    numBuySuccess = 0
    numSellSuccess = 0
    numUnknown = 0
    numOutstanding = 0
    numFailureCandle = 0
    numSuccessCandle = 0
    runningBuySuccess = 0.0
    runningSellSuccess = 0.0

    runningTotals = []
    trades = []
    midSlopes = []

    db,cursor = connectDatabase()
    for goes in range(48500):
        tradeable_epic_ids = []
        tradeable_epic_ids.append("GBPUSD")
        for epic_id in tradeable_epic_ids:

            theRow = goes
            open_prices, high_prices, low_prices, mid_prices, close_prices = getPrices(epic_id, theRow, 5, 16) ;
            copen_prices, chigh_prices, clow_prices, cmid_prices, cclose_prices = getPrices(epic_id, theRow, 5, 1000) ;
           
            current_low = clow_prices[16]
            clow = int(current_low)
            current_high = chigh_prices[16]
            chigh = int(current_high)
            #current_bid = cmid_prices[16]
            cmid = cmid_prices[16]
            current_bid = random.randint (clow, chigh)
            cbid = float(current_bid)
            #cbid = cmid

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


            midIntercept = float (mid_prices_intercept)
            midSlope = float (mid_prices_slope)

            midStopAt = midIntercept +  ((len(mid_prices) - 12) * midSlope)
            midLatest = midIntercept +  (len(mid_prices) * midSlope)

            ATR = calculate_stop_loss(cclose_prices, chigh_prices, clow_prices)


            TRADE_DIRECTION = "NONE"

            #if distance(current_bid, mid_prices_intercept) < -1:
            if (midSlope < -1.0):
                TRADE_DIRECTION = "SELL"
                #pip_limit = int(midSlope * -4.0)
                pip_limit1 = int(midLatest - cbid)
                pip_limit2 = int(abs(float(max(high_prices)) - float(current_bid)) / SL_MULTIPLIER)
                if (pip_limit1 > pip_limit2):
                    pip_limit = pip_limit1
                else:
                    pip_limit = pip_limit2

                stopDist = int(midIntercept - cbid)
                ce_stop = Chandelier_Exit_formula(TRADE_DIRECTION, ATR, min(low_prices))
                tmp_stop = int(abs(float(current_bid) - (ce_stop)))


            #elif distance(current_bid, mid_prices_intercept) > 1:
            elif (midSlope > 1.0):
                TRADE_DIRECTION = "BUY"
                pip_limit1 = int(cbid - midLatest)
                pip_limit2 = int(abs(float(min(low_prices)) - float(current_bid)) / SL_MULTIPLIER)
                if (pip_limit1 > pip_limit2):
                    pip_limit = pip_limit1
                else:
                    pip_limit = pip_limit2

                stopDist = int(cbid - midIntercept)
                ce_stop = Chandelier_Exit_formula(TRADE_DIRECTION, ATR, max(high_prices))
                tmp_stop = int(abs(float(current_bid) - (ce_stop)))


            else:
                pip_limit = 9999999
                tmp_stop = "999999"
                TRADE_DIRECTION = "NONE"

            if int(pip_limit) <= 10:
                TRADE_DIRECTION = "NONE"

            stopDistance_value = str(tmp_stop)
            stopDist = float(stopDistance_value)

            if int(stopDistance_value) <= LOW_SL_WATERMARK or int(stopDistance_value) >= HIGH_SL_WATERMARK:
                TRADE_DIRECTION = "NONE"

            betSize = int (pip_limit * 0.25)
            betSize = 1

            if (TRADE_DIRECTION != "NONE"):
                print ("theRow {} Direction {} Stop {} Limit {} Limit1 {} Limit2 {} betsize {}".format(theRow,TRADE_DIRECTION,stopDist,pip_limit,pip_limit1,pip_limit2,betSize))

            potProfit = betSize * (pip_limit  -1)
            potLoss = betSize * (stopDist + 1)
            #pip_limit = pip_limit + 1
            #stopDist = stopDist - 1

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
                    nmidPrice = random.randint (clow_prices[i], chigh_prices[i])
                    nmid_prices = numpy.asarray(nmid_prices)
                    xi = numpy.arange(0, len(nmid_prices))
                    nmid_prices_slope, nmid_prices_intercept, nmid_prices_r_value, nmid_prices_p_value, nmid_prices_std_err = stats.linregress(xi, nmid_prices)
                    nmidIntercept = float (nmid_prices_intercept)
                    nmidSlope = float (nmid_prices_slope)

                    if (nmidSlope < -1.0):
                        runningTotal = runningTotal - (betSize * (cbid - nmidPrice))
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure (GONE BAD) on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] >= (cbid + pip_limit) and clow_prices[i] > (cbid - stopDist)):
                        runningTotal = runningTotal + potProfit
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numSuccess = numSuccess + 1
                        numBuySuccess = numBuySuccess + 1
                        runningBuySuccess = runningBuySuccess + midSlope
                        numSuccessCandle = numSuccessCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade success on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] < (cbid + pip_limit) and clow_prices[i] < (cbid - stopDist)):
                        runningTotal = runningTotal - potLoss 
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (chigh_prices[i] >= (cbid + pip_limit) and clow_prices[i] < (cbid - stopDist)):
                        numUnknown = numUnknown + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade unknown on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (i == len(copen_prices) - 1):
                        numOutstanding = numOutstanding + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade still open on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
            elif (TRADE_DIRECTION == "SELL"):
                for i in range (len(copen_prices)):
                    if (i < 16):
                        continue
                    # this is the auto reverse stuff
                    nopen_prices, nhigh_prices, nlow_prices, nmid_prices, nclose_prices = getPrices(epic_id, i+theRow-16, 5, 16) ;
                    nmidPrice = int(cmid_prices[i])
                    nmidPrice = random.randint (clow_prices[i], chigh_prices[i])
                    nmid_prices = numpy.asarray(nmid_prices)
                    xi = numpy.arange(0, len(nmid_prices))
                    nmid_prices_slope, nmid_prices_intercept, nmid_prices_r_value, nmid_prices_p_value, nmid_prices_std_err = stats.linregress(xi, nmid_prices)
                    nmidIntercept = float (nmid_prices_intercept)
                    nmidSlope = float (nmid_prices_slope)

                    if (nmidSlope > 1.0):
                        runningTotal = runningTotal - (betSize * (nmidPrice - cbid))
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i - 16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure (GONE BAD) on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] <= (cbid - pip_limit) and chigh_prices[i] < (cbid + stopDist)):
                        runningTotal = runningTotal + potProfit
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numSuccess = numSuccess + 1
                        numSellSuccess = numSellSuccess + 1
                        runningSellSuccess = runningSellSuccess + midSlope
                        numSuccessCandle = numSuccessCandle + i - 16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade success on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] > (cbid - pip_limit) and chigh_prices[i] > (cbid + stopDist)):
                        runningTotal = runningTotal - potLoss
                        runningTotals.append(runningTotal)
                        numTrades = numTrades + 1
                        trades.append(numTrades)
                        midSlopes.append(mid_prices_slope)
                        numFailure = numFailure + 1
                        numFailureCandle = numFailureCandle + i -16
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade failure on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (clow_prices[i] <= (cbid - pip_limit) and chigh_prices[i] > (cbid + stopDist)):
                        numUnknown = numUnknown + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade unknown on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))
                        break
                    elif (i == len(copen_prices) - 1):
                        numOutstanding = numOutstanding + 1
                        print ("epic {} mid_prices_slope {} mid_prices_intercept {} mid_prices_r_value {} mid_prices_p_value {} mid_prices_std_err {}".format(
                                epic_id, mid_prices_slope, mid_prices_intercept, mid_prices_r_value, mid_prices_p_value, mid_prices_std_err))
                        print ("Trade still open on candle {} total {} numTrades {} success {} fail {} unknown {} outstanding {}".format(i-16,runningTotal,numTrades,numSuccess,numFailure,numUnknown,numOutstanding))

    db.close()
    try:
        print ("average success candle {} average failure candle {} averageBuySuccessSlope {} averageSellSuccessSlope {}".format(
                    numSuccessCandle / numSuccess,numFailureCandle /numFailure, runningBuySuccess / numBuySuccess, runningSellSuccess / numSellSuccess))
    except:
        pass

    #plt.plot(trades, runningTotals)
    #plt.show()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(15,5))
    ax1.plot(trades, runningTotals)
    ax2.plot(trades, midSlopes)
    plt.show()



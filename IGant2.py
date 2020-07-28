#IF YOU FOUND THIS USEFUL, Please Donate some Bitcoin .... 1FWt366i5PdrxCC6ydyhD8iywUHQ2C7BWC
#More Info here :-https://github.com/tg12/IG-Index-Scalping-Scraping-Bot
#And here :- https://www.reddit.com/r/UKInvesting/comments/7lx03l/python_ig_index_bot/

#!/usr/bin/env python
# -*- coding: utf-8 -*-
import time
import datetime
import requests
import json
import logging
import sys
import urllib
from time import time, sleep
import random
import time as systime
from statistics import mean, median


#Joke here
#REAL_OR_NO_REAL = 'https://api.ig.com/gateway/deal'
REAL_OR_NO_REAL = 'https://demo-api.ig.com/gateway/deal'

API_ENDPOINT = "https://demo-api.ig.com/gateway/deal/session"
API_KEY = '*****************'
data = {"identifier":"***********","password": "************"}

# FOR REAL....
# API_ENDPOINT = "https://api.ig.com/gateway/deal/session"
# API_KEY = '********************************************'
# data = {"identifier":"**************************","password": "***************************"}

headers = {'Content-Type':'application/json; charset=utf-8',
        'Accept':'application/json; charset=utf-8',
        'X-IG-API-KEY':API_KEY,
        'Version':'2'
		}

r = requests.post(API_ENDPOINT, data=json.dumps(data), headers=headers)

headers_json = dict(r.headers)
CST_token = headers_json["CST"]
print (R"CST : " + CST_token)
x_sec_token = headers_json["X-SECURITY-TOKEN"]
print (R"X-SECURITY-TOKEN : " + x_sec_token)

#GET ACCOUNTS
base_url = REAL_OR_NO_REAL + '/accounts'
authenticated_headers = {'Content-Type':'application/json; charset=utf-8',
        'Accept':'application/json; charset=utf-8',
        'X-IG-API-KEY':API_KEY,
        'CST':CST_token,
		'X-SECURITY-TOKEN':x_sec_token}

auth_r = requests.get(base_url, headers=authenticated_headers)
d = json.loads(auth_r.text)

# print(auth_r.status_code)
# print(auth_r.reason)
# print (auth_r.text)

for i in d['accounts']:
	if str(i['accountType']) == "SPREADBET":
		print ("Spreadbet Account ID is : " + str(i['accountId']))
		spreadbet_acc_id = str(i['accountId'])

#SET SPREAD BET ACCOUNT AS DEFAULT
base_url = REAL_OR_NO_REAL + '/session'
data = {"accountId":spreadbet_acc_id,"defaultAccount": "True"}
auth_r = requests.put(base_url, data=json.dumps(data), headers=authenticated_headers)

now = datetime.datetime.now()
now_time = now.time()

epic_id = []

#HACKY/Weekend Testing - DO NOT USE!!! UNLESS YOU KNOW WHAT YOU ARE DOING!!
#epic_id = "CS.D.BITCOIN.TODAY.IP" #Bitcoin
#epic_id = "IX.D.SUNFUN.DAILY.IP" #Weekend Trading
#epic_id = "CS.D.ETHUSD.TODAY.IP" #Ether
#epic_id.append("CS.D.BCHUSD.TODAY.IP") #Bitcoin Cash

#LIVE TEST

epic_id.append("CS.D.USCGC.TODAY.IP") #Gold - OK, Not Great
#epic_id.append("CS.D.USCSI.TODAY.IP") #Silver - NOT RECOMMENDED
#epic_id.append("IX.D.FTSE.DAILY.IP") #FTSE 100 - Within Hours only, Profitable
epic_id.append("IX.D.DOW.DAILY.IP") #Wall St - Definately Profitable between half 6 and half 8 GMT
epic_id.append("CS.D.GBPUSD.TODAY.IP") # - Very Profitable
epic_id.append("CS.D.EURUSD.TODAY.IP")
epic_id.append("CS.D.GBPEUR.TODAY.IP")
epic_id.append("CS.D.USDCAD.TODAY.IP")
#epic_id.append("IX.D.DAX.DAILY.IP") # Germany 30
#epic_id.append("CC.D.CL.USS.IP") # US Crude Oil
#epic_id.append("UA.D.AAPL.DAILY.IP") # Apple


# Function: loadPriceData
# Purpose: To load the Price data for a particular market
# Parameters:
#      id: The epic_id of the market to look at
#      period: One of MINUTE, MINUTE_2, MINUTE_3, MINUTE_5, MINUTE_10, MINUTE_15, MINUTE_30, HOUR, HOUR_2, HOUR_3, HOUR_4, DAY, WEEK, MONTH
#      num: The number of periods to load
# Returns:
#      the price data or False
def loadPriceData(id,period,num):
    try:
        base_url = REAL_OR_NO_REAL + '/markets/' + id
        base_url = "{}/prices/{}/{}/{}".format(REAL_OR_NO_REAL,id,period,num)
        r = requests.get(base_url, headers=authenticated_headers)
        d = json.loads(r.text)
        return d

    except Exception as e:
        print (e)
        return False

# Function: loadMarketData
# Purpose: To load the market data for a particular market
# Parameters:
#      id: The epic_id of the market to look at
# Returns:
#      the market data or False
def loadMarketData(id):
    try:
        base_url = REAL_OR_NO_REAL + '/markets/' + id
        r = requests.get(base_url, headers=authenticated_headers)
        d = json.loads(r.text)
        return d

    except Exception as e:
        print (e)
        return False

# Function: getMinStopLoss
# Purpose: To obtain the minimum stop value for a particular market
# Parameters:
#      id: The epic_id of the market to look at
# Returns:
#      the minimum stop value or False
def getMinStopLoss(id):
    d = loadMarketData (id)
    if (d == False):
        return (20.0)

    print (id)
    print (d)

    minStop = d['dealingRules']['minNormalStopOrLimitDistance']['value']
    return float(minStop)

# Function: printOpenPositions
# Purpose: prints out all markets for which we have a position open
# Parameters:
#      None
# Returns:
#      None
def printOpenPositions():
    try:
        base_url = REAL_OR_NO_REAL + '/positions'
        auth_r = requests.get(base_url, headers=authenticated_headers)
        d = json.loads(auth_r.text)
        for x in d['positions']:
            print (x['market']['epic'])

    except Exception as e:
        print (e)

# Function: checkForPosition
# Purpose: To check if we have a position open for a particular market
# Parameters:
#      id: The epic_id of the market to check
# Returns:
#      None: If there are no open positions
#      BUY: If there is a buy order
#      SELL: If there is sell order
#      BUYSELL: If there is both a buy and sell order
def checkForPosition (id):
    try:
        buy = 0
        sell = 0
        base_url = REAL_OR_NO_REAL + '/positions'
        auth_r = requests.get(base_url, headers=authenticated_headers)
        d = json.loads(auth_r.text)
        for x in d['positions']:
            #print (x['position']['direction'])
            if (x['market']['epic'] == id):
                if (x['position']['direction'] == "BUY"):
                    buy = 1
                elif (x['position']['direction'] == "SELL"):
                    sell = 1
        if (buy == 1 and sell == 1):
            return "BUYSELL"
        elif (buy == 1):
            return "BUY"
        elif (sell == 1):
            return "SELL"
        else:
            return "None"

    except Exception as e:
        print (e)
        return False

printOpenPositions()

def performTrade (id,direction,value):
    base_url = REAL_OR_NO_REAL + '/positions/otc'
    authenticated_headers = {'Content-Type':'application/json; charset=utf-8',
			'Accept':'application/json; charset=utf-8',
			'X-IG-API-KEY':API_KEY,
			'CST':CST_token,
			'X-SECURITY-TOKEN':x_sec_token}

    stopDistance_value = getMinStopLoss(x) * 2.0
    ldStr = "{}".format(limitDistance_value)
    sdStr = "{}".format(stopDistance_value)

    data = {"direction":direction,"epic": id, "limitDistance": ldStr, "orderType": "MARKET", "size":value,"expiry":"DFB","guaranteedStop":"False","currencyCode":"GBP","forceOpen":"True","stopDistance": sdStr}

    asdf = "Placing {} Order, LimitDistance is {}, StopDistance is {}".format(direction, ldStr, sdStr)
    print (asdf)

    r = requests.post(base_url, data=json.dumps(data), headers=authenticated_headers)
    # MAKE AN ORDER
    d = json.loads(r.text)
    deal_ref = d['dealReference']
    systime.sleep(5)
    #CONFIRM MARKET ORDER
    base_url = REAL_OR_NO_REAL + '/confirms/'+ deal_ref
    auth_r = requests.get(base_url, headers=authenticated_headers)
    d = json.loads(auth_r.text)
    DEAL_ID = d['dealId']
    print("DEAL ID : " + str(d['dealId']))
    print(d['dealStatus'])
    print(d['reason'])


while True:
    # We are going to try and create positions for every epic in our list
    for x in epic_id:
    # Don't go mad
        systime.sleep(5)
        # Only possibly create a position if we don't already have both a BUY and a SELL
        pos = checkForPosition(x)
        if (pos != "BUYSELL"):
            d = loadPriceData(x,"MINUTE_2",7)
            if (d == False):
                continue;

            price_list = []
            ltv_list = []
            for i in d['prices']:
                ask_price = i['closePrice']['ask']
                ltv = i['lastTradedVolume']
                if (ask_price > 0):
                    price_list.append(float(ask_price))
                    ltv_list.append(int(ltv))

            #---------------------------------
            firstValue = price_list[0]
            lastValue = price_list[-1]
            #---------------------------------
            Start_Trading_Volume = ltv_list[0]
            End_Trading_Volume = ltv_list[-1]
            #---------------------------------

            avg_price = mean(price_list)
            price_range = max(price_list) - min(price_list)
            avg_Volume = mean(ltv_list)

            limitDistance_value = price_range * 0.25

            #DEBUG
            #print ("-----------------")
            #print (firstValue)
            #print (lastValue)
            #print ("-----------------")
            #print ("Trade Volume : " + str(Start_Trading_Volume))
            #print ("Trade Volume : " + str(End_Trading_Volume))
            #print ("-----------------")

            if avg_Volume < End_Trading_Volume and avg_price > lastValue and pos != "SELL":
            #if Start_Trading_Volume < End_Trading_Volume and firstValue > lastValue:
            #if firstValue > lastValue:
                performTrade (x,"SELL",1)
                print ("Market: " + str(x))
                print ("Higher Volume")
                print ("Lower Price")
                print ("DIRECTION IS DOWN (SHORT)")

            elif avg_Volume <= End_Trading_Volume and avg_price < lastValue and pos != "BUY":
            #elif Start_Trading_Volume <= End_Trading_Volume and firstValue < lastValue:
            #elif firstValue <= lastValue:
                performTrade (x,"BUY",1)
                print ("Market: " + str(x))
                print ("Higher Volume")
                print ("Higher Price")
                print ("DIRECTION IS UP (LONG)")

            #elif int(avg_Volume) > int(End_Trading_Volume) and avg_price < lastValue:
            #elif int(Start_Trading_Volume) > int(End_Trading_Volume) and firstValue < lastValue:
                #Increasing price and decreasing volume show lack of interest, and this is a warning of a potential reversal.
                #performTrade (x,"SELL",1)
                #print ("Market: " + str(x))
                #print ("Higher Price")
                #print ("Lower Volume")
               #print ("DIRECTION IS DOWN (SHORT)")





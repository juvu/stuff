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
from alpha_vantage.techindicators import TechIndicators

# how much are we prepared to lose on a single trade
MAXSTOP = 15.0

#Joke here
#REAL_OR_NO_REAL = 'https://api.ig.com/gateway/deal'
REAL_OR_NO_REAL = 'https://demo-api.ig.com/gateway/deal'

ALPHA_VANTAGE_API_KEY = ['','']
alpha_vantage_url = 'https://www.alphavantage.co/query'

API_ENDPOINT = "https://demo-api.ig.com/gateway/deal/session"
API_KEY = ''
data = {"identifier":"","password": ""}

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

symbol_id = []


#HACKY/Weekend Testing - DO NOT USE!!! UNLESS YOU KNOW WHAT YOU ARE DOING!!
#epic_id = "CS.D.BITCOIN.TODAY.IP" #Bitcoin
#epic_id = "IX.D.SUNFUN.DAILY.IP" #Weekend Trading
#epic_id = "CS.D.ETHUSD.TODAY.IP" #Ether
#epic_id.append("CS.D.BCHUSD.TODAY.IP") #Bitcoin Cash

#LIVE TEST

#epic_id.append("CS.D.USCGC.TODAY.IP") #Gold - OK, Not Great
#epic_id.append("CS.D.USCSI.TODAY.IP") #Silver - NOT RECOMMENDED
#epic_id.append("IX.D.FTSE.DAILY.IP") #FTSE 100 - Within Hours only, Profitable
#epic_id.append("IX.D.DOW.DAILY.IP") #Wall St - Definately Profitable between half 6 and half 8 GMT
symbol_id.append("GBPUSD") # - Very Profitable
symbol_id.append("EURUSD")
symbol_id.append("GBPEUR")
symbol_id.append("USDCAD")
symbol_id.append("USDJPY")
symbol_id.append("AUDUSD")
symbol_id.append("EURJPY")
symbol_id.append("USDCHF")
symbol_id.append("EURCHF")
#epic_id.append("IX.D.DAX.DAILY.IP") # Germany 30
#epic_id.append("CC.D.CL.USS.IP") # US Crude Oil
#epic_id.append("UA.D.AAPL.DAILY.IP") # Apple

# Function: loadMACD
# Purpose: loads the MACD technical analysis data for a particular market
# Parameters:
#      symb: The symbol of the market to look at eg USDEUR
#      ival: One of 1min, 5min, 15min, 30min, 60min, daily, weekly, monthly
# Returns:
#      the price data or False
def loadMACD(symb,ival):
    try:
        apikey=random.randint(0,1)
        #print ("Key is {} ie {}".format(apikey,ALPHA_VANTAGE_API_KEY[apikey]))
        ti = TechIndicators(key='ALPHA_VANTAGE_API_KEY[apikey]', output_format='json')
        data, meta_data = ti.get_macd(symbol=symb, interval=ival, series_type='close', fastperiod=12, slowperiod=26, signalperiod=9)
        systime.sleep(15)
        return data

    except Exception as e:
        print (e)
        systime.sleep(15)
        return False

# Function: loadRSI
# Purpose: loads the RSI technical analysis data for a particular market
# Parameters:
#      symb: The symbol of the market to look at eg USDEUR
#      ival: One of 1min, 5min, 15min, 30min, 60min, daily, weekly, monthly
# Returns:
#      the price data or False
def loadRSI(symb,ival):
    try:
        apikey=random.randint(0,1)
        #print ("Key is {} ie {}".format(apikey,ALPHA_VANTAGE_API_KEY[apikey]))
        ti = TechIndicators(key='ALPHA_VANTAGE_API_KEY[apikey]', output_format='json')
        data, meta_data = ti.get_rsi(symbol=symb, interval=ival)
        systime.sleep(15)
        return data

    except Exception as e:
        print (e)
        systime.sleep(15)
        return False

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

    #print (id)
    #print (d)

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

    stopDistance_value = getMinStopLoss(id) * 3.0
    limitDistance_value = stopDistance_value
    #if (limitDistance_value > MAXSTOP):
        #limitDistance_value = MAXSTOP
    #if (stopDistance_value < MAXSTOP):
        #stopDistance_value = stopDistance_value * 3.0
        #if (stopDistance_value > MAXSTOP):
            #stopDistance_value = MAXSTOP

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

#macd_data = loadMACD('USDEUR', '15min')
#for macddate in macd_data:
    #asdf = "date {} MACD is {} MACD_Signal is {} MACD_Hist is {}".format(macddate, macd_data[macddate]['MACD'], macd_data[macddate]['MACD_Signal'], macd_data[macddate]['MACD_Hist'])
    #print (asdf)
    #print (macd_data[macddate]['MACD_Signal'])

#rsi_data = loadRSI('USDEUR', '15min')
#for rsidate in rsi_data:
    #asdf = "date {} RSI is {}".format(rsidate, rsi_data[rsidate]['RSI'])
    #print (asdf)

def getEpicId(symbol):
    asdf = "CS.D.{}.TODAY.IP".format(symbol)
    return (asdf)

# Function: strategy3
# Purpose: bet with the market if hist > 0.0003 and RSI shows moderate/strong activity
#          or if hist < -0.0003 and RSI shows lack of interest
# Parameters:
#      macd_data: The macd data
#      rsi_data: The rsi data
#      x: The symbol id
# Returns:
#      action: BUY, SELL or None
def strategy3(macd_data, rsi_data, x):
    hist = 0.0
    rsi = 50.0
    for macdate in macd_data:
        hist = float(macd_data[macdate]['MACD_Hist'])
        break;
    for rsidate in rsi_data:
        rsi = float(rsi_data[rsidate]['RSI'])
        break;

    # japan currency histogram values are 100 times bigger for some reason
    if (x == 'USDJPY' or x == 'EURJPY'):
        hist = hist * 0.01

    action = "None"
    if (hist <= -0.0003 and rsi < 40.0 and rsi > 30.0):
        action = "SELL"
    elif (hist >= 0.0003 and rsi > 60.0 and rsi < 70.0):
        action = "BUY"

    asdf = "Strategy3: Symbol {} hist is {} rsi is {} action is {}".format(x,hist,rsi,action)
    print (asdf)

    return (action)

# Function: strategy2
# Purpose: Reversal stragegy MACD histogram goes negative to positive (or vice versa)
# Parameters:
#      macd_data: The macd data
#      rsi_data: The rsi data
#      x: The symbol id
# Returns:
#      action: BUY, SELL or None
def strategy2(macd_data, rsi_data, x):
    hist = [0.0,0.0]
    count = 0

    for macdate in macd_data:
        hist[count] = float(macd_data[macdate]['MACD_Hist'])
        count = count + 1
        if (count >= 2):
            break;

    action = "None"
    if (hist[1] == 0.0 and hist[0] == 0.0):
        action = "None"
    elif (hist[1] <= 0.0 and hist[0] >= 0.0):
        action = "BUY"
    elif (hist[1] >= 0.0 and hist[0] <= 0.0):
        action = "SELL"

    asdf = "Strategy2: Symbol {} hist[0] is {} hist[1] is {} action is {}".format(x,hist[0],hist[1],action)
    print (asdf)

    return (action)

# Function: strategy1
# Purpose: Reversal stragegy MACD histogram negative and RSI > 50
#          or MACD histogram positive and RSI < 50
#          Also bet with the market if hist > 0.0003 and RSI shows moderate/strong activity
# Parameters:
#      macd_data: The macd data
#      rsi_data: The rsi data
#      x: The symbol id
# Returns:
#      action: BUY, SELL or None
def strategy1(macd_data, rsi_data, x):
    hist = 0.0
    rsi = 50.0
    for macdate in macd_data:
        hist = float(macd_data[macdate]['MACD_Hist'])
        break;
    for rsidate in rsi_data:
        rsi = float(rsi_data[rsidate]['RSI'])
        break;

    # japan currency histogram values are 100 times bigger for some reason
    if (x == 'USDJPY' or x == 'EURJPY'):
        hist = hist * 0.01

    action = "None"
    if (hist > 0.0 and rsi < 50.0):
        action = "BUY"
    elif (hist < 0.0 and rsi > 50.0):
        action = "SELL"

    asdf = "Strategy1: Symbol {} hist is {} rsi is {} action is {}".format(x,hist,rsi,action)
    print (asdf)

    return (action)

while True:
    # We are going to try and create positions for every epic in our list
    for x in symbol_id:
        epic_id = getEpicId (x)
        action = "None"
        pos = checkForPosition(epic_id)

        if (pos != "BUYSELL"):
            macd_data = loadMACD(x, '15min')
            if (macd_data == False):
                continue;

            rsi_data = loadRSI(x, '15min')
            if (rsi_data == False):
                continue;

            action = strategy1(macd_data, rsi_data, x)
            if (action == "None"):
                action = strategy2(macd_data, rsi_data, x)
            if (action == "None"):
                action = strategy3(macd_data, rsi_data, x)

            if (pos == "None"):
                if (action == "BUY"):
                    performTrade (epic_id, "BUY", 1)
                elif (action == "SELL"):
                    performTrade (epic_id, "SELL", 1)
            elif (pos == "BUY"):
                if (action == "SELL"):
                    performTrade (epic_id, "SELL", 1)
            elif (pos == "SELL"):
                if (action == "BUY"):
                    performTrade (epic_id, "BUY", 1)

        systime.sleep(40)






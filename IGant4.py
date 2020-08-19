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
import oandapy
from statistics import mean, median
from alpha_vantage.techindicators import TechIndicators

# how much are we prepared to lose on a single trade
MAXSTOP = 15.0

#Joke here
#REAL_OR_NO_REAL = 'https://api.ig.com/gateway/deal'
REAL_OR_NO_REAL = 'https://demo-api.ig.com/gateway/deal'

ALPHA_VANTAGE_API_KEY = ['','']
alpha_vantage_url = 'https://www.alphavantage.co/query'

OANDA_API_KEY = ""

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

f = open("symbols.txt","r")
for x in f:
    sym = x.rstrip()
    symbol_id.append(sym)



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

# Function: loadEMA
# Purpose: loads the EMA technical analysis data for a particular market
# Parameters:
#      symb: The symbol of the market to look at eg USDEUR
#      ival: One of 1min, 5min, 15min, 30min, 60min, daily, weekly, monthly
#      num: time_period eg 20 for ema20, 50 for ema50
# Returns:
#      the price data or False
def loadEMA(symb,ival,num):
    try:
        apikey=random.randint(0,1)
        #print ("Key is {} ie {}".format(apikey,ALPHA_VANTAGE_API_KEY[apikey]))
        ti = TechIndicators(key='ALPHA_VANTAGE_API_KEY[apikey]', output_format='json')
        data, meta_data = ti.get_ema(symbol=symb, interval=ival, time_period=num)
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
        systime.sleep(5)
        return d

    except Exception as e:
        print (e)
        systime.sleep(5)
        return False

# Function: getPriceInfo
# Purpose: To load the Price data for a particular market and get avgPrice, lastPrice, lowPrice and highPrice
# Parameters:
#      id: The epic_id of the market to look at
#      period: One of MINUTE, MINUTE_2, MINUTE_3, MINUTE_5, MINUTE_10, MINUTE_15, MINUTE_30, HOUR, HOUR_2, HOUR_3, HOUR_4, DAY, WEEK, MONTH
#      num: The number of periods to load
# Returns:
#      avgPrice, lastPrice, lowPrice, highPrice, spread
def getPriceInfo(id,period,num):
    total = 0
    highPrice = -1.0
    lowPrice = -1.0
    lastPrice = -1.0
    price_data = loadPriceData (id, period, num)
    for pricedate in price_data['prices']:
        #print (pricedate['snapshotTime'])
        total = total + pricedate['closePrice']['ask']
        spread = pricedate['closePrice']['ask'] - pricedate['closePrice']['bid']
        lastPrice = pricedate['closePrice']['ask']
        if (lastPrice > highPrice):
            highPrice = lastPrice
        if (lowPrice < 0.0 or lastPrice < lowPrice):
            lowPrice = lastPrice

    avgPrice = total / num

    return avgPrice, lastPrice, lowPrice, highPrice, spread


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

# Function: getMinDealSize
# Purpose: To obtain the minimum deal size for a particular market
# Parameters:
#      id: The epic_id of the market to look at
# Returns:
#      the minimum deal size or False
def getMinDealSize(id):
    d = loadMarketData (id)
    if (d == False):
        return (1.0)

    #print (id)
    #print (d)

    minDeal = d['dealingRules']['minDealSize']['value']
    return float(minDeal)

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
            print ("position BUYSELL {}".format(id))
            return "BUYSELL"
        elif (buy == 1):
            print ("position BUY {}".format(id))
            return "BUY"
        elif (sell == 1):
            print ("position SELL {}".format(id))
            return "SELL"
        else:
            print ("position None {}".format(id))
            return "None"

    except Exception as e:
        print (e)
        return False

printOpenPositions()

def performTrade (id,direction,interval,value):
    base_url = REAL_OR_NO_REAL + '/positions/otc'
    authenticated_headers = {'Content-Type':'application/json; charset=utf-8',
			'Accept':'application/json; charset=utf-8',
			'X-IG-API-KEY':API_KEY,
			'CST':CST_token,
			'X-SECURITY-TOKEN':x_sec_token}

    minStop = getMinStopLoss(id)
    minDeal = getMinDealSize(id)

    # let's try doing the opposite of what we thought
    #if (direction == 'BUY'):
    #    direction = 'SELL'
    #else:
    #    direction = 'BUY'

    # we will use the price movements over the last 30 periods
    # to determine sensible limit and stop
    period = "DAY"
    if (interval == 15):
        period = "MINUTE_15"
    elif (interval == 30):
        period = "MINUTE_30"
    elif (interval == 60):
        period = "HOUR"
    elif (interval == 240):
        period = "HOUR_4"
    avgPrice,lastPrice,lowPrice, highPrice,spread = getPriceInfo(id,period,30)
    minStop = minStop + spread

    # set limit to about a third of the range
    limitDistance = (highPrice - lowPrice)/3.0

    # place our stop at the outside of the trading range
    stopDistance = 0.0
    if (direction == 'BUY'):
        stopDistance = lastPrice - lowPrice

    if (direction == 'SELL'):
            stopDistance = highPrice - lastPrice

    if (stopDistance < minStop):
        stopDistance = minStop
    if (stopDistance > 50.0):
        stopDistance = 50.0

    ldStr = "{}".format(limitDistance)
    sdStr = "{}".format(stopDistance)

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
    systime.sleep(15)

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

# Function: strategy4
# Purpose: price above ema20 above ema50 is a BUY
#          price below ema20 below ema50 is a SELL
# Parameters:
#      ema20_data: The ema20 data
#      ema50_data: The ema50 data
#      price_data: The price data
#      x: The symbol id
# Returns:
#      action: BUY, SELL or None
def strategy4(ema20_data, ema50_data, lastPrice, x):
    hist = 0.0
    rsi = 50.0
    for ema20date in ema20_data:
        ema20 = float(ema20_data[ema20date]['EMA'])
        break;
    for ema50date in ema50_data:
        ema50 = float(ema50_data[ema50date]['EMA'])
        break;

    # for units difference
    if (x.find("JPY") > 0):
    #if (x == 'EURJPY' or x == 'USDJPY'):
        lastPrice = lastPrice * 0.01
    else:
        lastPrice = lastPrice * 0.0001

    action = 0
    if (lastPrice > ema20 and ema20 > ema50):
        action = 1
    if (lastPrice < ema20 and ema20 < ema50):
        action = -1

    asdf = "Strategy4: Symbol {} ema20 is {} ema50 is {} lastPrice is {} action is {}".format(x,ema20,ema50,lastPrice,action)
    print (asdf)

    return (action)

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
    if (x.find("JPY") > 0):
    #if (x == 'USDJPY' or x == 'EURJPY'):
        hist = hist * 0.01

    action = 0
    if (hist <= -0.0003 and rsi < 40.0 and rsi > 30.0):
        action = -1
    elif (hist >= 0.0003 and rsi > 60.0 and rsi < 70.0):
        action = 1

    asdf = "Strategy3: Symbol {} hist is {} rsi is {} action is {}".format(x,hist,rsi,action)
    print (asdf)

    return (action)

# Function: strategy2
# Purpose: Reversal strategy MACD histogram goes negative to positive (or vice versa)
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

    action = 0
    if (hist[1] == 0.0 and hist[0] == 0.0):
        action = 0
    elif (hist[1] <= 0.0 and hist[0] >= 0.0):
        action = 1
    elif (hist[1] >= 0.0 and hist[0] <= 0.0):
        action = -1

    asdf = "Strategy2: Symbol {} hist[0] is {} hist[1] is {} action is {}".format(x,hist[0],hist[1],action)
    print (asdf)

    return (action)

# Function: strategy1
# Purpose: Reversal strategy MACD histogram negative and RSI > 50
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
    if (x.find("JPY") > 0):
    #if (x == 'USDJPY' or x == 'EURJPY'):
        hist = hist * 0.01

    action = 0
    if (hist > 0.0 and rsi < 50.0):
        action = 1
    elif (hist < 0.0 and rsi > 50.0):
        action = -1

    asdf = "Strategy1: Symbol {} hist is {} rsi is {} action is {}".format(x,hist,rsi,action)
    print (asdf)

    return (action)

def randomStrategy(x):
    num = random.randint(0,100)
    action = 0
    if (num < 10):
        action = 1
    elif (num > 90):
        action = -1

    asdf = "randomStrategy: Symbol {} action is {}".format(x,action)
    print (asdf)

    return (action)

#while True:
# We are going to try and create positions for every epic in our list
def oldMethod():
    for x in symbol_id:
        epic_id = getEpicId (x)
        action = "None"
        pos = "None"
        # we are not going to allow multiple trades
        pos = checkForPosition(epic_id)

        if (pos != "BUYSELL"):

            macd_data = loadMACD(x, 'daily')
            if (macd_data == False):
                continue;

            rsi_data = loadRSI(x, 'daily')
            if (rsi_data == False):
                continue;

            ema20_data = loadEMA(x, 'daily', 20)
            if (ema20_data == False):
                continue;

            ema50_data = loadEMA(x, 'daily', 50)
            if (ema50_data == False):
                continue;

            avgPrice,lastPrice,lowPrice, highPrice,spread = getPriceInfo(epic_id,'DAY',30)

            totAction = 0

            action = strategy1(macd_data, rsi_data, x)
            totAction = totAction + action

            action = strategy2(macd_data, rsi_data, x)
            totAction = totAction + action

            action = strategy3(macd_data, rsi_data, x)
            totAction = totAction + action

            action = strategy4(ema20_data, ema50_data, lastPrice,x)
            totAction = totAction + action

            #action = randomStrategy(x)

            if (pos == "None"):
                if (totAction >= 2):
                    performTrade (epic_id, "BUY", 1440, 1)
                elif (totAction <= -2):
                    performTrade (epic_id, "SELL", 1440, 1)
            elif (pos == "BUY"):
                if (totAction <= -2):
                    performTrade (epic_id, "SELL", 1440, 1)
            elif (pos == "SELL"):
                if (totAction >= 2):
                    performTrade (epic_id, "BUY", 1440, 1)

        systime.sleep(5)


def autochartist():
    oanda = oandapy.API(environment="practice", access_token=OANDA_API_KEY)

    recommended =[]
    response = oanda.get_autochartist(instruments="EUR_USD")
    signals = response.get("signals")
    for x in signals:
        inst = x.get("instrument")
        instrument = inst.replace('_','')
        recommended.append(instrument)
        meta = x.get("meta")
        if (meta):
            #print (meta)
            probability = meta.get("probability")
            interval = meta.get("interval")
            if (interval == 0):
                interval = 1440
            scores = meta.get("scores")
            quality = scores.get("quality")
            direction = meta.get("direction")
        data = x.get("data")
        if (data):
            prediction = data.get("prediction")
            if (prediction):
                #print (prediction)
                pricelow = prediction.get("pricelow")
                timefrom = prediction.get("timefrom")

        print ("{} direction {} interval {} probability {} quality {} timefrom {}".format(instrument, direction, interval, probability, quality, timefrom))

        for y in symbol_id:
            if (y == instrument):
                if (probability > 65.0 and quality >= 6):
                    epic_id = getEpicId (y)
                    pos = "None"
                    # we are not going to allow multiple trades
                    pos = checkForPosition(epic_id)

                    if (pos != "BUYSELL"):
                        if (pos == "None"):
                            if (direction == 1):
                                performTrade (epic_id, "BUY", interval, 1)
                            elif (direction == -1):
                                performTrade (epic_id, "SELL", interval, 1)
                        elif (pos == "BUY"):
                            if (direction == -1):
                                performTrade (epic_id, "SELL", interval, 1)
                        elif (pos == "SELL"):
                            if (direction == 1):
                                performTrade (epic_id, "BUY", interval, 1)

    return (recommended)

def getOpenPositions ():
    try:
        base_url = REAL_OR_NO_REAL + '/positions'
        auth_r = requests.get(base_url, headers=authenticated_headers)
        d = json.loads(auth_r.text)
        return (d['positions'])

    except Exception as e:
        print (e)
        return False

def closePosition(deal,direction,sized):
    base_url = REAL_OR_NO_REAL + '/positions/otc'
    data = {"dealId":deal,"direction":direction,"size":sized,"orderType":'MARKET'}
    #authenticated_headers_delete IS HACKY AF WORK AROUND!! AS PER .... https://labs.ig.com/node/36
    authenticated_headers_delete = {'Content-Type':'application/json; charset=utf-8',
				'Accept':'application/json; charset=utf-8',
				'X-IG-API-KEY':API_KEY,
				'CST':CST_token,
				'X-SECURITY-TOKEN':x_sec_token,
				'_method':"DELETE"}

    r = requests.post(base_url, data=json.dumps(data), headers=authenticated_headers_delete)
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

def closeUnrecommended(recommended):
    #positions = getOpenPositions()
    base_url = REAL_OR_NO_REAL + '/positions'
    auth_r = requests.get(base_url, headers=authenticated_headers)
    d = json.loads(auth_r.text)
    for positions in d['positions']:
        found = 0
        pos_epic = positions['market']['epic']
        deal = positions['position']['dealId']
        dir = positions['position']['direction']
        if (dir == "BUY"):
            direction = "SELL"
        else:
            direction = "BUY"
        #sized = positions['position']['size']

        for y in recommended:
            epic_id = getEpicId (y)
            if (epic_id == pos_epic):
                found = 1

        if (found == 0):
            systime.sleep(10)
            print("Deleting order {} epic {}".format(deal,pos_epic))
            closePosition(deal,direction,1)

# Let's use the new method
while True:
    recommended = autochartist()
    systime.sleep(15)
    closeUnrecommended(recommended)
    for count in range (15):
        print ("Countdown {}".format(900 - (count*60)))
        systime.sleep(60)







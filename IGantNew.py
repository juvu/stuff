
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import time
import datetime
from datetime import timezone
import requests
import json
import logging
import sys
import urllib
from time import time, sleep
import random
import time as systime
import oandapy
import oandapyV20
import oandapyV20.endpoints.forexlabs as labs
from statistics import mean, median

#REAL_OR_NO_REAL = 'https://api.ig.com/gateway/deal'
REAL_OR_NO_REAL = 'https://demo-api.ig.com/gateway/deal'

OANDA_API_KEY = ""

API_ENDPOINT = "https://demo-api.ig.com/gateway/deal/session"
API_KEY = ''
data = {"identifier":"","password": ""}

headers = {'Content-Type':'application/json; charset=utf-8',
        'Accept':'application/json; charset=utf-8',
        'X-IG-API-KEY':API_KEY,
        'Version':'2'
		}

r = requests.post(API_ENDPOINT, data=json.dumps(data), headers=headers)

headers_json = dict(r.headers)
CST_token = headers_json["CST"]
print ("CST : " + CST_token)
x_sec_token = headers_json["X-SECURITY-TOKEN"]
print ("X-SECURITY-TOKEN : " + x_sec_token)

#GET ACCOUNTS
base_url = REAL_OR_NO_REAL + '/accounts'
authenticated_headers = {'Content-Type':'application/json; charset=utf-8',
        'Accept':'application/json; charset=utf-8',
        'X-IG-API-KEY':API_KEY,
        'CST':CST_token,
		'X-SECURITY-TOKEN':x_sec_token}

auth_r = requests.get(base_url, headers=authenticated_headers)
d = json.loads(auth_r.text)

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
blacklist = []

f = open("symbols.txt","r")
for x in f:
    sym = x.rstrip()
    symbol_id.append(sym)

def performTradeAbsolute (id,direction,stop,limit,value):

    # first check this epic is not blacklisted
    try:
        blacklisted = blacklist.index(id)
    except:
        blacklisted = -1

    if (blacklisted != -1):
        print ("Epic {} is blacklisted, returning".format(id))
        return

    # now we can add to the blacklist until next restart
    blacklist.append(id)

    # and now we can get on with placing the order
    base_url = REAL_OR_NO_REAL + '/positions/otc'
    authenticated_headers = {'Content-Type':'application/json; charset=utf-8',
			'Accept':'application/json; charset=utf-8',
			'X-IG-API-KEY':API_KEY,
			'CST':CST_token,
			'X-SECURITY-TOKEN':x_sec_token}

    if (id.find("JPY") > 0):
        stop = stop * 100.0
        limit = limit * 100.0
    else:
        stop = stop * 10000.0
        limit = limit * 10000.0

    data = {"direction":direction,"epic": id, "limitLevel": limit, "orderType": "MARKET", "size":value,"expiry":"DFB","guaranteedStop":"False","currencyCode":"GBP","forceOpen":"True","stopLevel": stop}
    print (data)
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


def getEpicId(symbol):
    asdf = "CS.D.{}.TODAY.IP".format(symbol)
    return (asdf)

def getPrices():
    oanda = oandapy.API(environment="practice", access_token=OANDA_API_KEY)

    lsymbol_id = []

    f = open("symbolsLong.txt","r")
    for x in f:
        sym = x.rstrip()
        lsymbol_id.append(sym)

    for x in lsymbol_id:
        systime.sleep(1)
        response = oanda.get_prices(instruments=x)
        prices = response.get("prices")
        asking_price = prices[0].get("ask")
        print ("Symbol {} price {}".format(x, asking_price))

def autochartist():

    dt = datetime.datetime.now()
    utc_time = dt.replace(tzinfo = timezone.utc)
    utc_timestamp = int(utc_time.timestamp())

    oanda = oandapy.API(environment="practice", access_token=OANDA_API_KEY)
    oandaV20 = oandapyV20.API(environment="practice", access_token=OANDA_API_KEY)

    recommended =[]
    #response = oanda.get_autochartist()
    r = labs.Autochartist()
    oandaV20.request(r)
    response = r.response
    signals = response.get("signals")
    for x in signals:
        inst = x.get("instrument")
        instrument = inst.replace('_','')
        recommended.append(instrument)
        pricelow = 0.0
        pricehigh = 0.0
        probability = 0.0
        meta = x.get("meta")
        if (meta):
            #print (meta)
            probability = meta.get("probability")
            interval = meta.get("interval")
            if (interval == 0):
                interval = 1440

            scoresTotal = 0
            quality = 0

            scores = meta.get("scores")
            if (scores):
                quality = scores.get("quality")
                clarity = scores.get("clarity")
                initialTrend = scores.get("initialtrend")
                breakout = scores.get("breakout")
                uniformity = scores.get("uniformity")
                try:
                    scoresTotal = quality + clarity + initialTrend + breakout + uniformity
                except:
                    pass

            direction = meta.get("direction")

        data = x.get("data")
        if (data):
            prediction = data.get("prediction")
            if (prediction):
                #print (prediction)
                pricelow = prediction.get("pricelow")
                pricehigh = prediction.get("pricehigh")
                timefrom = prediction.get("timefrom")
                timeto = prediction.get("timeto")

        print ("{} direction {} interval {} probability {} quality {} scoresTotal {} pricelow {} pricehigh {} age {}".format(instrument,
                                    direction, interval, probability, quality, scoresTotal, pricelow, pricehigh, utc_timestamp - timeto))
        if (pricelow > 0.0):
            for y in symbol_id:
                if (y == instrument):
                    #if (probability > 65.0 and quality >= 6):
                    if (probability > 75.0 or quality >= 6):
                        response = oanda.get_prices(instruments=inst)
                        prices = response.get("prices")
                        asking_price = prices[0].get("ask")
                        print ("Current price {}, pricelow {}, pricehigh{}".format(asking_price,pricelow,pricehigh))

                        limit = 0
                        stop = 0

                        midprice = (pricehigh - pricelow)/2 + pricelow
                        if (direction == 1 and midprice > asking_price):
                            limit = midprice
                            stop = asking_price - ((midprice - asking_price)/2)
                        elif (direction == -1 and midprice < asking_price):
                            limit = midprice
                            stop = asking_price + ((asking_price - midprice)/2)

                        if (limit > 0):
                            epic_id = getEpicId (y)

                            deal,oldStop = getDealId (epic_id, direction)
                            print ("Deal is {} oldStop is {} stop is {}".format(deal,oldStop,stop))

                            if (deal == "None"):
                                if (direction == 1 and asking_price < pricelow):
                                    performTradeAbsolute (epic_id, "BUY", stop, limit, 1)
                                elif (direction == -1 and asking_price > pricehigh):
                                    performTradeAbsolute (epic_id, "SELL", stop, limit, 1)
                            else:
                                if (direction == 1 and oldStop > stop):
                                    stop = oldStop
                                elif (direction == -1 and oldStop < stop):
                                    stop = oldStop
                                if (stop != oldStop):
                                    updatePosition (deal, y, stop, limit)


    print ("Current time is {}".format(utc_timestamp))
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

def updatePosition(deal,id,stop,limit):
    print ("Updating position stop {} limit {}".format(stop,limit))
    base_url = "{}/positions/otc/{}".format(REAL_OR_NO_REAL,deal)

    if (id.find("JPY") > 0):
        stop = stop * 100.0
        limit = limit * 100.0
    else:
        stop = stop * 10000.0
        limit = limit * 10000.0

    data = {"limitLevel":limit,"stopLevel":stop}
    r = requests.put(base_url, data=json.dumps(data), headers=authenticated_headers)
    d = json.loads(r.text)
    #deal_ref = d['dealReference']
    systime.sleep(15)

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
    systime.sleep(15)

def getDealId(epic,dir):

    if (dir == 1):
        direction = "BUY"
    else:
        direction = "SELL"

    base_url = REAL_OR_NO_REAL + '/positions'
    auth_r = requests.get(base_url, headers=authenticated_headers)
    d = json.loads(auth_r.text)
    systime.sleep(15)
    for positions in d['positions']:
        found = 0
        pos_epic = positions['market']['epic']
        pos_deal = positions['position']['dealId']
        pos_dir = positions['position']['direction']
        pos_stop = positions['position']['stopLevel']

        if (epic.find("JPY") > 0):
            pos_stop = pos_stop / 100.0
        else:
            pos_stop = pos_stop / 10000.0

        if (pos_epic == epic and pos_dir == direction):
            return (pos_deal,pos_stop)

    return ("None","None")

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
        print ("position dir is {}".format(dir))
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
            print("Deleting order {} epic {}".format(deal,pos_epic))
            closePosition(deal,direction,1)
            # now we can remove this from the blacklist
            try:
                blacklist.remove(pos_epic)
            except:
                pass

def sanitiseBlacklist(recommended):
    for x in symbol_id:
        found = 0
        for y in recommended:
            if (x == y):
                found = 1

        if (found == 0):
            epic_id = getEpicId (x)

            # now we can remove this from the blacklist
            try:
                blacklist.remove(epic_id)
            except:
                pass

def writeBlacklist(blacklist):
    try:
        f = open("blacklisted.txt", "w")
        for x in blacklist:
            f.write (x)
            f.write ("\n")
    except:
        print ("Cannot write the blacklist file")

def readBlacklist():
    blacklist = []
    try:
        f = open("blacklisted.txt", "r")
        for x in f:
            bl = x.rstrip()
            blacklist.append (bl)
    except:
        print ("No blacklist file")

    return (blacklist)

# Let's use the new method
while True:
    blacklist = readBlacklist()
    recommended = autochartist()
    closeUnrecommended(recommended)
    sanitiseBlacklist(recommended)
    writeBlacklist(blacklist)

    # autochartist list updates every 15 mins
    # but we may update the stop so run every 2 mins
    for count in range (2):
        print ("Countdown {}".format(120 - (count*60)))
        systime.sleep(60)







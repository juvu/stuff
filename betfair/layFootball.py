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
import readAccount

import MySQLdb
import re
from typing import Iterable, Dict, Union, List

MINODDS = 1.65
MAXODDS = 1.9
BAILODDS = 1.15
BAILMINODDS = 1.09
BAILLAYODDS = 1.24

bet_url="https://api.betfair.com/exchange/betting/json-rpc/v1"
acceptStr = "application/json"

def myprint(x):
    print(x)
    sys.stdout.flush()

def getSSOID():
    payload = 'username=' + my_username + '&password=' + my_password
    headers = {'X-Application': my_app_key, 'Content-Type': 'application/x-www-form-urlencoded'}
    resp = requests.post('https://identitysso-cert.betfair.com/api/certlogin',data=payload,cert=('client-2048.crt','client-2048.key'),headers=headers)
    json_resp=resp.json()
    SSOID = json_resp['sessionToken']
    return SSOID


def keepAlive(SSOID):
    headers = {'Accept': acceptStr, 'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    resp = requests.post('https://identitysso-cert.betfair.com/api/keepAlive',headers=headers)
    json_resp=resp.json()
    myprint (json_resp['status'])


def getBetSize(SSOID):
    account_url="https://api.betfair.com/exchange/account/json-rpc/v1"
    headers = {'Accept': acceptStr, 'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    user_req='{"jsonrpc": "2.0", "method": "AccountAPING/v1.0/getAccountFunds"}'

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    user_req = '{"jsonrpc": "2.0", "method": "AccountAPING/v1.0/getAccountFunds", "params": {"wallet":"UK"}, "id": 1}'
    req = requests.post(account_url, data=user_req.encode('utf-8'), headers=headers)
    accountInfo = req.json()
    result = accountInfo["result"]
    available = float(result['availableToBetBalance'])
    exposure = float(result['exposure'])
    total = available - exposure
    betsize = total * 0.01
    if (betsize < 2.0):
        betsize = 2.0
    if (betsize > 20.0):
        betsize = 20.0
    myprint ("Betsize is {} available is {} exposure is {}".format(betsize,available,exposure*-1.0))
    return (betsize)

def CheckBackBet(SSOID,market,selection):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listCurrentOrders"}'

    #myprint (user_req)
    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    #myprint (result)

    orders = result['result']['currentOrders']
    for x in range(len(orders)):
        if (orders[x]['marketId'] == market):
            side = orders[x]['side']
            horse = orders[x]['selectionId']
            if (side == "BACK" and str(horse) == selection or selection == "AnyHorse"):
                if (str(orders[x]['status']) == "EXECUTABLE"):
                    return ("Unmatched",horse)
                else:
                    betPlaced = str("%.2f" % orders[x]['averagePriceMatched'])
                    return (str(betPlaced),horse)


    return ("No","None")

def CheckLayBet(SSOID,market,selection):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listCurrentOrders"}'

    #myprint (user_req)
    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    #myprint (result)

    orders = result['result']['currentOrders']
    for x in range(len(orders)):
        if (orders[x]['marketId'] == market):
            side = orders[x]['side']
            horse = orders[x]['selectionId']
            if (side == "LAY" and str(horse) == selection or selection == "AnyHorse"):
                if (str(orders[x]['status']) == "EXECUTABLE"):
                    return ("Unmatched",horse)
                else:
                    betPlaced = str("%.2f" % orders[x]['averagePriceMatched'])
                    return (str(betPlaced),horse)


    return ("No","None")

def PlaceLayBet(SSOID,market,horse,price,betsize):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/placeOrders", \
            "params": {"marketId":"'+market+'",\
            "instructions":[{"selectionId":"'+horse+'","handicap":"0","side":"LAY","orderType":"LIMIT","limitOrder":{"size":"'+betsize+'","timeInForce":"FILL_OR_KILL","minFillSize":"'+betsize+'","price":"'+price+'"}}]}, "id": 1}'

    #myprint (user_req)

    placed,thehorse = CheckLayBet(SSOID,market,"AnyHorse")
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        myprint ("Placing lay bet on {}".format(horse))
        myprint (result)
    else:
        pass
        myprint ("You already have a lay bet on that player\n")

def getLiability(SSOID,market,selection):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listCurrentOrders"}'

    #myprint (user_req)
    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    #myprint (result)

    total = 0.0
    original = 0.0
    originalLay = 0.0
    orders = result['result']['currentOrders']
    for x in range(len(orders)):
        if (orders[x]['marketId'] == market):
            side = str(orders[x]['side'])
            horse = orders[x]['selectionId']
            if (side == "BACK" and str(horse) == selection):
                if (str(orders[x]['status']) != "EXECUTABLE"):
                    size = orders[x]['priceSize']['size']
                    price = orders[x]['averagePriceMatched']
                    liability = size * (price - 1.0)
                    total = total - liability
                    #myprint ("Side {} Size {} price {} liability {} total {}".format(side,size,price,liability,total))
            if (side == "LAY" and str(horse) == selection):
                if (str(orders[x]['status']) != "EXECUTABLE"):
                    size = orders[x]['priceSize']['size']
                    price = orders[x]['averagePriceMatched']
                    liability = size * (price - 1.0)
                    total = total + liability
                    original = original + liability
                    originalLay = originalLay + size
                    #myprint ("Side {} Size {} price {} liability {} total {}".format(side,size,price,liability,total))


    return (total, original, originalLay)

def PlaceBackBet(SSOID,market,horse,price,betsize):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/placeOrders", \
            "params": {"marketId":"'+market+'",\
            "instructions":[{"selectionId":"'+horse+'","handicap":"0","side":"BACK","orderType":"LIMIT","limitOrder":{"size":"'+betsize+'","timeInForce":"FILL_OR_KILL","minFillSize":"'+betsize+'","price":"'+price+'"}}]}, "id": 1}'

    #myprint (user_req)

    placed,thehorse = CheckBackBet(SSOID,market,horse)
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        myprint ("Placing back bet on {}".format(horse))
        myprint (result)
    else:
        pass
        myprint ("You already have a back bet on that player\n")


def getMarketCatalogue(SSOID):
    eventTypeID = '["1"]' #ID for Football
    countryCode= '["GB"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["MATCH_ODDS"]' #Market Type

    MarketStartTime = (datetime.datetime.now() - datetime.timedelta(hours=3))
    MarketStartTime = MarketStartTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(hours=1))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')

    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    priceProjection = '["EX_BEST_OFFERS"]'#Best odds


    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}

    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listMarketCatalogue",\
            "params": {"filter":{"eventTypeIds":'+eventTypeID+',"marketTypeCodes":'+marketTypeCode+',\
           "marketStartTime":{"from":"'+MarketStartTime+'", "to":"'+MarketEndTime+'"}},\
           "sort":"'+sortType+'", "maxResults":"'+maxResults+'", "marketProjection":["'+Metadata+'","MARKET_START_TIME","EVENT"]}, "id": 1}'

    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    #myprint (result)
    marketCatalogue = result['result']

    return (marketCatalogue)

def getMarketStatus(SSOID, market):
    priceProjection = '["EX_ALL_OFFERS"]'#Best odds
    marketId = str(market['marketId'])
    layList = []
    layValueList = []
    horseList = []
    horseBackList = []
    sortedList = []

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}

    #myprint (marketId)
    for w in range(len(market['runners'])):
        marketID = market['marketName']
        if (marketID != "Match Odds"):
            continue
        eventID = market['runners'][w]['runnerName']
        if (eventID != "The Draw"):
            continue
        handicap = market['runners'][w]['handicap']
        selectionID = market['runners'][w]['selectionId']

        #myprint ("{} {} {}".format(marketID,eventID,handicap))
        price_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listRunnerBook", "params": {"locale":"en", \
                        "marketId":"'+str(market['marketId'])+'",\
                        "selectionId":"'+str(selectionID)+'",\
                        "priceProjection":{"priceData":'+priceProjection+'},"orderProjection":"ALL"},"id":1}'

        #myprint (price_req)
        req = urllib.request.Request(bet_url, data=price_req.encode('utf-8'), headers=headers)
        price_response= urllib.request.urlopen(req)
        price_jsonResponse = price_response.read()
        price_pkg = price_jsonResponse.decode('utf-8')
        price_result = json.loads(price_pkg)
        back = 1000.0
        lay = 1000.0
        size = 0.0
        try:
            size = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['size'])
            back = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])
            lay = float(price_result['result'][0]['runners'][0]['ex']['availableToLay'][0]['price'])
            myprint ("{} {} back is {} lay is {} size is {}".format(marketID,eventID,back,lay,size))
        except:
            back = 1000.0
            lay = 1000.0
            size = 0.0

        dictItem = {"selectionID":selectionID, "back" : back, "size" : size, "lay" : lay}
        horseBackList.append(dictItem)

    sortedList = sorted(horseBackList, key = lambda i: i['lay'])
    #myprint (sortedList)

    return (sortedList)

# main starts here

delay_key,my_username,my_password,antsBetfairBot,my_app_key = readAccount.readAccount()

pid = os.getpid()
print (pid)

f = open ("laySnookerPID.txt","w")
f.write(str(pid))
f.close()

SSOID = getSSOID()
myprint (SSOID)
doit = 1
while (doit == 1):
    curhour = int(datetime.datetime.now().strftime('%H'))
    curmin = int(datetime.datetime.now().strftime('%M'))
    if (curhour > 22 and curmin > 15):
        doit = 0
    if (curhour < 8 and curmin > 15):
        doit = 0
    timenow = datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ')
    myprint (timenow)

    marketList = getMarketCatalogue(SSOID)
    for hrow in range(len(marketList)):
        layList = getMarketStatus(SSOID, marketList[hrow])
        #myprint (layList)
        for item in layList:
            #myprint (item)
            if (item['lay'] <= MAXODDS and item['lay'] >= MINODDS):
                bet = getBetSize (SSOID)
                betAmount = "{:.2f}".format(bet)
                #myprint ("Market {} Player {} odds {} bet {}".format(marketList[hrow]['marketId'],str(item['selectionID']),str(item['lay']),betAmount))
                myprint ("Placing lay bet")
                odds = "{:.2f}".format(item['lay'])
                PlaceLayBet(SSOID,str(marketList[hrow]['marketId']),str(item["selectionID"]),odds,str(betAmount))
            if (item['back'] <= BAILODDS and item['back'] >= BAILMINODDS and item['lay'] <= BAILLAYODDS):
                liability,original,originalLay = getLiability(SSOID, str(marketList[hrow]['marketId']), str(item["selectionID"]))

                if (liability > 0.0):
                    bet = liability / (item['back'] - 1.0)
                    betAmount = "{:.2f}".format(bet)
                    myprint ("BAILING Market {} Player {} odds {} bet {} liability {}".format(marketList[hrow]['marketId'],str(item['selectionID']),str(item['back']),betAmount, liability))
                    myprint ("Placing back bet")
                    odds = "{:.2f}".format(item['back'])
                    PlaceBackBet(SSOID,str(marketList[hrow]['marketId']),str(item["selectionID"]),odds,str(betAmount))

    keepAlive(SSOID)
    time.sleep(5)



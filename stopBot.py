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

import MySQLdb
import re
from typing import Iterable, Dict, Union, List

my_app_key = ""
bet_url="https://api.betfair.com/exchange/betting/json-rpc/v1"
acceptStr = "application/json"

def myprint(x):
    print(x)
    sys.stdout.flush()

def getSSOID():
    my_username = ""
    my_password = ""

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
    #myprint ("Betsize is {}".format(betsize))
    return (betsize)

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
            side = str(orders[x]['side'])
            horse = orders[x]['selectionId']
            if (side == "BACK" and str(horse) == selection or selection == "AnyHorse"):
                if (str(orders[x]['status']) == "EXECUTABLE"):
                    return ("Unmatched",horse)
                else:
                    betPlaced = str("%.2f" % orders[x]['averagePriceMatched'])
                    return (str(betPlaced),horse)


    return ("No","None")


def PlaceBet(SSOID,market,horse,price,betsize):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/placeOrders", \
            "params": {"marketId":"'+market+'",\
            "instructions":[{"selectionId":"'+horse+'","handicap":"0","side":"LAY","orderType":"LIMIT","limitOrder":{"size":"'+betsize+'","price":"'+price+'"}}]}, "id": 1}'

    #myprint (user_req)

    placed,thehorse = CheckLayBet(SSOID,market,horse)
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        #myprint ("Placing bet on {}".format(horse))
        #myprint (result)
    else:
        pass
        myprint ("You already have a bet on that horse\n")

def PlaceBackBet(SSOID,market,horse,price,betsize):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/placeOrders", \
            "params": {"marketId":"'+market+'",\
            "instructions":[{"selectionId":"'+horse+'","handicap":"0","side":"BACK","orderType":"LIMIT","limitOrder":{"size":"'+betsize+'","price":"'+price+'"}}]}, "id": 1}'

    #myprint (user_req)

    placed,thehorse = CheckBackBet(SSOID,market,horse)
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        #myprint ("Placing bet on {}".format(horse))
        #myprint (result)
    else:
        pass
        myprint ("You already have a bet on that horse\n")



def getEvents(SSOID):
    event_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listEventTypes", "params": {"filter":{ }}, "id": 1}'
    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    req = requests.post(bet_url, data=event_req.encode('utf-8'), headers=headers)
    eventTypes = req.json()
    #myprint (eventTypes)

def getMarketCatalogue(SSOID):
    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(minutes=5))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    priceProjection = '["EX_ALL_OFFERS"]'#Best odds


    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}

    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listMarketCatalogue",\
           "params": {"filter":{"eventTypeIds":'+eventTypeID+',"marketTypeCodes":'+marketTypeCode+',\
           "marketCountries":'+countryCode+',\
           "marketStartTime":{"from":"'+MarketStartTime+'", "to":"'+MarketEndTime+'"}},\
           "sort":"'+sortType+'", "maxResults":"'+maxResults+'", "marketProjection":["'+Metadata+'","MARKET_START_TIME","EVENT"]}, "id": 1}'

    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    marketCatalogue = result['result']

    backList = []
    marketList = []
    horseList = []
    for x in range(len(marketCatalogue)):
        marketId = str(marketCatalogue[x]['marketId'])
        #myprint (marketId)
        for w in range(len(marketCatalogue[x]['runners'])):
            selectionID = marketCatalogue[x]['runners'][w]['selectionId']
            betPlaced,horseid = CheckLayBet(SSOID,marketId,str(selectionID))
            #myprint ("Horse {} betPlaced {}".format (betPlaced,horseid))
            if (selectionID == horseid and betPlaced != "Unmatched"):
                marketList.append(marketId)
                horseList.append(selectionID)

                price_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listRunnerBook", "params": {"locale":"en", \
                        "marketId":"'+str(marketCatalogue[x]['marketId'])+'",\
                        "selectionId":"'+str(selectionID)+'",\
                        "priceProjection":{"priceData":'+priceProjection+'},"orderProjection":"ALL"},"id":1}'

                #myprint (price_req)
                req = urllib.request.Request(bet_url, data=price_req.encode('utf-8'), headers=headers)
                price_response= urllib.request.urlopen(req)
                price_jsonResponse = price_response.read()
                price_pkg = price_jsonResponse.decode('utf-8')
                price_result = json.loads(price_pkg)
                #myprint (price_result)
    
                try:
                    back = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])
                except:
                    back = 1000.0

                backList.append(back)


    return (marketList,backList,horseList)

# main starts here

pid = os.getpid()
print (pid)

f = open ("stopBotPID.txt","w")
f.write(str(pid))
f.close()

SSOID = getSSOID()
myprint (SSOID)
doit = 1
while (doit == 1):
    keepAlive(SSOID)
    TheBetSize = getBetSize(SSOID)
    curhour = int(datetime.datetime.now().strftime('%H'))
    curmin = int(datetime.datetime.now().strftime('%M'))
    if (curhour > 21 and curmin > 15):
        doit = 0
    if (curhour < 10 and curmin > 15):
        doit = 0
    timenow = datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ')
    myprint (timenow)

    marketList,backList,horseList = getMarketCatalogue(SSOID)
    numBets = len(horseList)
    for hrow in range(len(horseList)):
        myprint ("Horse {} back {}".format(horseList[hrow], backList[hrow]))
        if (backList[hrow] < 6.0):
            PlaceBackBet(SSOID,str(marketList[hrow]),str(horseList[hrow]),"2.0",str(TheBetSize))


    del marketList
    del backList
    del horseList

    if (numBets > 0):
        time.sleep(1)
    else:
        time.sleep(60)



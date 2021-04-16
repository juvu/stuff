import json
import sys
import datetime
import time
import os
import pytz
import urllib
import urllib.request
import urllib.error
import requests
import pandas as pd
import readAccount

import MySQLdb
import re
from typing import Iterable, Dict, Union, List

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
    betsize = 2.0
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

def PlaceLayBet(SSOID,market,horse,price,betsize):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/placeOrders", \
            "params": {"marketId":"'+market+'",\
            "instructions":[{"selectionId":"'+horse+'","handicap":"0","side":"LAY","orderType":"LIMIT","limitOrder":{"size":"'+betsize+'","price":"'+price+'"}}]}, "id": 1}'

    #myprint (user_req)

    placed,thehorse = CheckLayBet(SSOID,market,"AnyHorse")
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        myprint ("Placing bet on {}".format(horse))
        myprint (result)
    else:
        pass
        myprint ("You already have a bet on that horse\n")


def getMarketCatalogue(SSOID):
    eventTypeID = '["4339"]' #ID for Greyhounds
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type

    timezone = pytz.timezone("Europe/London")
    nowTime= datetime.datetime.now()
    nowTime = timezone.localize(nowTime)
    nowTime = nowTime.strftime('%Y-%m-%dT%H:%M:%S%Z')
    bst = nowTime.find("BST")
    if (bst > 0):
        MarketStartTime = (datetime.datetime.now() - datetime.timedelta(hours=1))
        MarketEndTime = (datetime.datetime.now() -datetime.timedelta(hours=1) + datetime.timedelta(seconds=20))
    else:
        MarketStartTime= datetime.datetime.now()
        MarketEndTime = (datetime.datetime.now() + datetime.timedelta(seconds=20))

    MarketStartTime = MarketStartTime.strftime('%Y-%m-%dT%H:%M:%S%Z')
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%S%Z')
    myprint ("Start {} End {}".format(MarketStartTime, MarketEndTime))

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
        selectionID = market['runners'][w]['selectionId']
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
        #myprint (price_result)
        try:
            lay = float(price_result['result'][0]['runners'][0]['ex']['availableToLay'][0]['price'])
            back = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])
        except:
            lay = 1000.0
            back = 1000.0

        dictItem = {"selectionID":selectionID, "lay" : lay, "back" : back}
        horseBackList.append(dictItem)

    sortedList = sorted(horseBackList, key = lambda i: i['lay'])
    #myprint (sortedList)

    return (sortedList)

def winCalculation (totalBet, numHorses, horselist):
    oddsTotal = 0.0
    totsum = 0.0
    betList = []

    for x in range(numHorses):
        oddsTotal = oddsTotal + horselist[x]['back'] 

    for x in range(numHorses):
        betAmount = oddsTotal / horselist[x]['back'] 
        betList.append(betAmount)
        totsum += betAmount

    totsum = totsum / totalBet

    for x in range(numHorses):
        betList[x] = betList[x] / totsum

    profit = betList[0] * horselist[0]['back'] - totalBet

    return (profit, betList)

# main starts here

delay_key,my_username,my_password,antsBetfairBot,my_app_key = readAccount.readAccount()

pid = os.getpid()
print (pid)

f = open ("layFavGPID.txt","w")
f.write(str(pid))
f.close()

SSOID = getSSOID()
myprint (SSOID)
doit = 1
while (doit == 1):
    curhour = int(datetime.datetime.now().strftime('%H'))
    curmin = int(datetime.datetime.now().strftime('%M'))
    if (curhour > 22 and curmin > 15):
        doit = 1
    if (curhour < 8 and curmin > 15):
        doit = 1
    timenow = datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ')
    myprint (timenow)

    marketList = getMarketCatalogue(SSOID)
    #myprint (marketList)
    numBets = 0
    for hrow in range(len(marketList)):
        layList = getMarketStatus(SSOID, marketList[hrow])
        myprint (layList)
        try:
            item = layList[5]
            profit, betList = winCalculation (10.0, 3, layList)
            myprint ("Profit is {}".format(profit))
            myprint (item)
            if (profit < 4.0):
                numBets = 1
                bet = getBetSize (SSOID)
                betAmount = "{:.2f}".format(bet)
                myprint ("Market {} Dog {} odds {}".format(marketList[hrow]['marketId'],str(item['selectionID']),str(item['lay'])))
                PlaceLayBet(SSOID,str(marketList[hrow]['marketId']),str(item["selectionID"]),"24.0",str(betAmount))
        except:
            pass

    if (numBets == 0):
        keepAlive(SSOID)
        time.sleep(10)
    else:
        time.sleep(10)



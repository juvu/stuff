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

import readAccount

bet_url="https://api.betfair.com/exchange/betting/json-rpc/v1"
acceptStr = "application/json"

def connectDatabase():
    print("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost', db='test', user='msgmedia', passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    return (db, cursor)

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
    betsize = total * 0.003
    if (betsize < 4.0):
        betsize = 4.0
    if (betsize > 20.0):
        betsize = 20.0
    #myprint ("Betsize is {}".format(betsize))
    return (betsize)

def CheckBet(SSOID,market,selection):

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
            horse = orders[x]['selectionId']
            if (str(horse) == selection or selection == "AnyHorse"):
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

    placed,thehorse = CheckBet(SSOID,market,horse)
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

    placed,thehorse = CheckBet(SSOID,market,horse)
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



def HorseForm(SSOID,marketId):

    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(minutes=2))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    inplay = 'false' #still to run
    priceProjection = '["EX_ALL_OFFERS"]'#Best odds

    #Create an empty dataframe
    d = {'Horse Name': [], 'Forecast': [], 'Form':[], 'Race': [], 'Time': [], 'Venue': [], 'Rating': [], 'Odds':[], 'Bet Placed':[], 'Bet Horse':[]}
    pd.set_option('display.max_columns', None)
    pd.set_option('display.max_colwidth', None)
    pd.set_option('expand_frame_repr', False)
    pd.set_option('display.max_rows', None)
    Results = pd.DataFrame(data=d)

    reduce = 0.0
    betHorse = "None"
    betPlaced = "No"
    horseList = []
    marketList = []
    priceList = []
    backList = []
    forecastList = []
    horsenameList = []
    betHorseList = []
    betPlacedList = []
    formList = []
    raceList = []
    venueList = []
    timeList = []
    ratingList = []

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}

    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listMarketCatalogue",\
           "params": {"filter":{"eventTypeIds":'+eventTypeID+',"marketTypeCodes":'+marketTypeCode+',\
           "inPlayOnly":'+inplay+', "marketCountries":'+countryCode+',\
           "marketStartTime":{"from":"'+MarketStartTime+'", "to":"'+MarketEndTime+'"}},\
           "sort":"'+sortType+'", "maxResults":"'+maxResults+'", "marketProjection":["'+Metadata+'","MARKET_START_TIME","EVENT"]}, "id": 1}'

    #myprint (user_req)
    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    marketCatalogue = result['result']

    for x in range(len(marketCatalogue)):
        if (marketId != str(marketCatalogue[x]['marketId'])):
            continue


        for w in range(len(marketCatalogue[x]['runners'])):
            runnerform = marketCatalogue[x]['runners'][w]['metadata']['FORM']
            try:
                runnerRating = int (marketCatalogue[x]['runners'][w]['metadata']['OFFICIAL_RATING'])
            except:
                runnerRating = 1000
            horsename = marketCatalogue[x]['runners'][w]['runnerName']
            selectionID = marketCatalogue[x]['runners'][w]['selectionId']
            numerator = float (marketCatalogue[x]['runners'][w]['metadata']['FORECASTPRICE_NUMERATOR'])
            denominator = float (marketCatalogue[x]['runners'][w]['metadata']['FORECASTPRICE_DENOMINATOR'])
            try:
                forecast = (numerator/denominator) + 1.0
            except:
                forecast = 10.0 # we don't have the info about this horse so assume 10/1 for reduction purposes

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

            #myprint (horsename)
            start_time = marketCatalogue[x]['marketStartTime']
            my_datetime = datetime.datetime.strptime(start_time, '%Y-%m-%dT%H:%M:%S.000Z')
            StartTime = my_datetime.strftime('%H:%M')
            venue = marketCatalogue[x]['event']['venue']
            try:
                price = float(price_result['result'][0]['runners'][0]['ex']['availableToLay'][0]['price'])
            except:
                price = "NR"
                reduce = reduce + (100.0 / forecast)
            try:
                back = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])
            except:
                back = "NR"

            marketId = str(marketCatalogue[x]['marketId'])
            betPlaced,horseid = CheckBet(SSOID,marketId,str(selectionID))
            betHorse = "None"
            for ll in range(len(marketCatalogue[x]['runners'])):
                hname = marketCatalogue[x]['runners'][ll]['runnerName']
                sID = marketCatalogue[x]['runners'][ll]['selectionId']
                if (sID == horseid):
                    betHorse = hname

            betHorseList.append(betHorse)
            betPlacedList.append(betPlaced)
            horsenameList.append(horsename)
            formList.append(runnerform)
            raceList.append(marketCatalogue[x]['marketName'])
            horseList.append(selectionID)
            marketList.append(marketId)
            priceList.append(price)
            backList.append(back)
            venueList.append(venue)
            timeList.append(StartTime)
            ratingList.append(runnerRating)

        for w in range(len(marketCatalogue[x]['runners'])):
            numerator = float (marketCatalogue[x]['runners'][w]['metadata']['FORECASTPRICE_NUMERATOR'])
            denominator = float (marketCatalogue[x]['runners'][w]['metadata']['FORECASTPRICE_DENOMINATOR'])
            try:
                forecast = (numerator/denominator) + 1.0
            except:
                forecast = 0.0

            if (forecast > 1.0):
                prob = 1.0 / forecast
                prob = prob * (1.0 + (reduce * 0.01))
                forecast = 1.0 / prob
                aforecast = str("%.2f" % forecast)
                forecast = float(aforecast)

            forecastList.append(forecast)

            Results = Results.append({'Horse Name':str(horsenameList[w]), 'Forecast':str(forecast), 'Form':str(formList[w]), 'Race':str(raceList[w]), 'Time':str(timeList[w]), 'Venue':str(venueList[w]), 'Rating':str(ratingList[w]), 'Odds':str(priceList[w]), 'Bet Placed':betPlacedList[w], 'Bet Horse':betHorseList[w] }, ignore_index=True)

    return (Results,horseList,marketList,priceList,forecastList,ratingList,backList,raceList,formList)

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
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(minutes=2))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    inplay = 'false' #still to run

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}

    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listMarketCatalogue",\
           "params": {"filter":{"eventTypeIds":'+eventTypeID+',"marketTypeCodes":'+marketTypeCode+',\
           "inPlayOnly":'+inplay+', "marketCountries":'+countryCode+',\
           "marketStartTime":{"from":"'+MarketStartTime+'", "to":"'+MarketEndTime+'"}},\
           "sort":"'+sortType+'", "maxResults":"'+maxResults+'", "marketProjection":["'+Metadata+'","MARKET_START_TIME","EVENT"]}, "id": 1}'

    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    marketCatalogue = result['result']

    #Create an empty dataframe
    d = {'Venue': [], 'Start Time': [], 'Horse': []}
    pd.set_option('display.max_columns', None)
    pd.set_option('display.max_colwidth', None)
    pd.set_option('expand_frame_repr', False)
    pd.set_option('display.max_rows', None)
    Results = pd.DataFrame(data=d)

    marketList = []
    venueList = []
    timeList = []
    for x in range(len(marketCatalogue)):
        start_time = marketCatalogue[x]['marketStartTime']
        my_datetime = datetime.datetime.strptime(start_time, '%Y-%m-%dT%H:%M:%S.000Z')
        StartTime = my_datetime.strftime('%H:%M')
        venue = marketCatalogue[x]['event']['venue']
        marketId = str(marketCatalogue[x]['marketId'])
        marketList.append(marketId)
        venueList.append(venue)
        timeList.append(StartTime)

        betPlaced,horseid = CheckBet(SSOID,marketId,"AnyHorse")
        betHorse = "None"
        for ll in range(len(marketCatalogue[x]['runners'])):
            hname = marketCatalogue[x]['runners'][ll]['runnerName']
            sID = marketCatalogue[x]['runners'][ll]['selectionId']
            if (sID == horseid):
                betHorse = hname

        Results = Results.append({'Venue':str(venue), 'Start Time':str(StartTime), 'Horse':str(betHorse)}, ignore_index=True)

    return (Results,marketList,venueList,timeList)

# main starts here

my_app_key,my_username,my_password,antsBetfairBot = readAccount.readAccount()

pid = os.getpid()
print (pid)

f = open ("newBotPID.txt","w")
f.write(str(pid))
f.close()

SSOID = getSSOID()
myprint (SSOID)
db,cursor = connectDatabase()
sql = "SELECT * FROM BBACK ORDER BY BACK"
cursor.execute(sql)
BackResults = cursor.fetchall()
sql = "SELECT * FROM FFORECAST ORDER BY FORECAST"
cursor.execute(sql)
ForecastResults = cursor.fetchall()
sql = "SELECT * FROM FFORM ORDER BY FORM"
cursor.execute(sql)
FormResults = cursor.fetchall()
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
    cresults,marketList,venueList,timeList = getMarketCatalogue(SSOID)
    myprint (cresults)
    myprint ("\n")
    numMarkets = 0
    for x in marketList:
        numMarkets = numMarkets + 1
        results,horses,markets,prices,forecasts,ratings,backs,races,forms = HorseForm(SSOID,str(x))
        myprint (results)
        myprint ("\n")
        runners = 0
        for hrow in range(len(horses)):
            if (prices[hrow] != "NR"):
                runners = runners + 1

        for hrow in range(len(horses)):
            if (prices[hrow] == "NR"):
                continue

            back = backs[hrow]
            count = 0
            lastback = 0.0
            lastbackscore = 0.0
            found = 0
            bscore = 1.0
            for row in BackResults:
                if (back == row[0]):
                    bscore = row[1]
                    break
                elif (back > lastback and back < row[0]):
                    bscore = (row[1] + lastbackscore) / 2.0
                    break
                lastback = row[0]
                lastbackscore = row[1]

            #myprint ("back score is {}".format(bscore))

            forecast = forecasts[hrow]
            count = 0
            lastforecast = 0.0
            lastforecastscore = 0.0
            found = 0
            forecastscore = 1.0
            for row in ForecastResults:
                if (forecast == row[0]):
                    forecastscore = row[1]
                    break
                elif (forecast > lastforecast and forecast < row[0]):
                    forecastscore = (row[1] + lastforecastscore) / 2.0
                    break
                lastforecast = row[0]
                lastforecastscore = row[1]

            #myprint ("forecast score is {}".format(forecastscore))

            count = 0.0
            ratingsort = sorted(ratings)
            #print (ratingsort)
            rating = ratings[hrow]
            total = len(ratings)
            for row in range(len(ratingsort)):
                if (rating > ratingsort[row]):
                    count = count + 1.0
                elif (rating == ratingsort[row]):
                    count = count + 0.5

            ratingscore = 1.0 - (float(count) / float(total))
            #myprint ("Rating is {} Rating score is {}".format(rating, ratingscore))

            ratioscore = float(back) / float(forecast)
            #myprint ("Ratio score is {}".format(ratioscore))

            score = bscore + ratingscore + forecastscore + ratioscore
            score = bscore + forecastscore + ratioscore

            #index = races[hrow].find("Hcap")
            #if (index < 0):
            #    continue

            #myprint ("Score is {}".format(score))
            #if (score > 1.55 and score < 2.1 and prices[hrow] < 8.0):

            form = forms[hrow]

            #if (score > 1.2 and prices[hrow] < forecast and prices[hrow] < 8.0 and form[-1] != '2' and form[-1] != '1'):
            try:
                #if (score > 1.2 and prices[hrow] < forecast and prices[hrow] < 8.0 and forecast > 8.0 and forecast < 50.0 and form[-1] != '2' and form[-1] != '1'):
                #if (prices[hrow] < 9.5 and forecast > 9.0):
                #if (back >= 8.8 and back < 20.1 and forecast >= 5.8 and forecast <= 24.6 and runners >= 10 and runners <= 16):
                if (back > 20.7 and back < 26.7 and forecast > 10.0 and forecast < 82.9):
                #if (back >= 12.0 and back <= 27.5 and forecast > 2.1 and forecast < 23.9):
                    #fAmount = (score * 2.0) 
                    #fAmount = (score - 1.0) * 10.0 
                    #fAmount = (forecast - prices[hrow]) + 2.0
                    fAmount = TheBetSize
                    fAmount = fAmount * 100.0
                    iAmount = int(fAmount)
                    fAmount = float(iAmount) / 100.0
                    price = prices[hrow] + 0.2
                    price = price * 100.0
                    iprice = int(price)
                    price = float(iprice) / 100.0
                    myprint ("Placing bet {} {} {} {} {}\n".format(str(hrow), str(markets[hrow]), str(horses[hrow]), str(price), str(fAmount)))
                    #PlaceBet (SSOID, str(markets[hrow]), str(horses[hrow]), str(prices[hrow]), str(fAmount))
                    PlaceBet (SSOID, str(markets[hrow]), str(horses[hrow]), "30.0", str(fAmount))
                    #PlaceBackBet (SSOID, str(markets[hrow]), str(horses[hrow]), "8.8", str(fAmount))
            except:
                pass
    
        del results
        del markets
        del horses
        del prices
        del forecasts
        del ratings
        del backs

    del cresults
    del marketList
    del venueList
    del timeList

    time.sleep(60)



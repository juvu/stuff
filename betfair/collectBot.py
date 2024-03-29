import json
import sys
import os
import datetime
import time
import urllib
import urllib.request
import urllib.error
import requests
import pandas as pd
import readAccount

bet_url="https://api.betfair.com/exchange/betting/json-rpc/v1"
acceptStr = "application/json"

import MySQLdb
import re
from typing import Iterable, Dict, Union, List

def connectDatabase():
    print("Connecting to database using MySQLdb")
    db = MySQLdb.connect(host='localhost', db='betfair', user='msgmedia', passwd='Media123!')
    print("Succesfully Connected to database using MySQLdb!")

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # Drop table if it already exist using execute() method.
    #cursor.execute("DROP TABLE IF EXISTS CASES")
    #db.commit()

    # Create table as per requirement
    sql = """CREATE TABLE RESULTS (
         MARKETID  CHAR(20) NOT NULL,
         SELECTIONID  CHAR(20) NOT NULL,
         NAME CHAR (60),
         FORECAST FLOAT,
         FORM CHAR(20),
         RACE CHAR(60),
         DATE  DATE,
         TIME  TIME,
         VENUE CHAR(60),
         RATING INT,
         BACK FLOAT,
         LAY FLOAT,
         RESULT CHAR(20),
         PRIMARY KEY (MARKETID, SELECTIONID))"""

    #cursor.execute(sql)
    # Commit your changes in the database
    #db.commit()

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


def CheckBet(SSOID,market):

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

    placed,horse = CheckBet(SSOID,market)
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        #myprint (result)
    else:
        pass
        myprint ("You already have a bet in that market\n")



def HorseForm(SSOID,marketId):

    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(minutes=30))
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
                back = float(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])
            except:
                price = "NR"
                back = "NR"
                reduce = reduce + (100.0 / forecast)

            marketId = str(marketCatalogue[x]['marketId'])
            betPlaced,horseid = CheckBet(SSOID,marketId)
            betHorse = "None"
            for ll in range(len(marketCatalogue[x]['runners'])):
                hname = marketCatalogue[x]['runners'][ll]['runnerName']
                sID = marketCatalogue[x]['runners'][ll]['selectionId']
                if (sID == horseid):
                    betHorse = hname

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

            Results = Results.append({'Horse Name':str(horsenameList[w]), 'Forecast':str(forecast), 'Form':str(formList[w]), 'Race':str(raceList[w]), 'Time':str(timeList[w]), 'Venue':str(venueList[w]), 'Rating':str(ratingList[w]), 'Odds':str(priceList[w]), 'Bet Placed':betPlaced, 'Bet Horse':betHorse }, ignore_index=True)

    return (Results,horseList,marketList,priceList,backList,forecastList,formList,horsenameList,raceList,ratingList)

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
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(minutes=30))
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
    dateList = []
    for x in range(len(marketCatalogue)):
        start_time = marketCatalogue[x]['marketStartTime']
        my_datetime = datetime.datetime.strptime(start_time, '%Y-%m-%dT%H:%M:%S.000Z')
        StartTime = my_datetime.strftime('%H:%M')
        StartDate = my_datetime.strftime('%Y-%m-%d')
        venue = marketCatalogue[x]['event']['venue']
        marketId = str(marketCatalogue[x]['marketId'])
        marketList.append(marketId)
        venueList.append(venue)
        timeList.append(StartTime)
        dateList.append(StartDate)

        betPlaced,horseid = CheckBet(SSOID,marketId)
        betHorse = "None"
        for ll in range(len(marketCatalogue[x]['runners'])):
            hname = marketCatalogue[x]['runners'][ll]['runnerName']
            sID = marketCatalogue[x]['runners'][ll]['selectionId']
            if (sID == horseid):
                betHorse = hname

        Results = Results.append({'Venue':str(venue), 'Start Time':str(StartTime), 'Horse':str(betHorse)}, ignore_index=True)

    return (Results,marketList,venueList,timeList,dateList)

my_app_key,my_username,my_password,antsBetfairBot,live_key = readAccount.readAccount()

pid = os.getpid()
print (pid)

f = open ("collectBotPID.txt","w")
f.write(str(pid))
f.close()

SSOID = getSSOID()
myprint (SSOID)
db,cursor = connectDatabase()

doit = 1
while (doit == 1):
    keepAlive(SSOID)
    curhour = int(datetime.datetime.now().strftime('%H'))
    curmin = int(datetime.datetime.now().strftime('%M'))
    if (curhour > 21 and curmin > 15):
        doit = 0
    if (curhour < 10 and curmin > 15):
        doit = 0

    timenow = datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ')
    myprint (timenow)
    cresults,marketList,venueList,timeList,dateList = getMarketCatalogue(SSOID)
    myprint (cresults)
    myprint ("\n")
    for x in range(len(marketList)):
        results,horses,markets,prices,backs,forecasts,forms,horsenames,races,ratings = HorseForm(SSOID,str(marketList[x]))
        myprint (results)
        myprint ("\n")
        sys.stdout.flush()
        BestFactor = 0.0
        BestHorseID = "None"
        BestMarketID = "None"
        BestPrice = 0.0
        BestForecast = 0.0
        venue = venueList[x]
        thedate = dateList[x]
        thetime = timeList[x]
        for row in range(len(horses)):
            horseID = horses[row]
            marketID = markets[row]
            price = prices[row]
            back = backs[row]
            forecast = forecasts[row]
            if (type(price) is float):
                sql = "INSERT INTO RESULTS(MARKETID, SELECTIONID, FORECAST, BACK, LAY, DATE, TIME, VENUE, NAME, FORM, RACE, RATING) \
                VALUES ('%s', '%s', '%s', '%s', '%s' , '%s', '%s', '%s', '%s', '%s', '%s', '%s') ON DUPLICATE KEY UPDATE LAY = '%s', BACK = '%s'" % \
                    (str(marketID), str(horseID), str(forecast), str(back), str(price), str(thedate), str(thetime), str(venue), str(horsenames[row]), str(forms[row]), str(races[row]), str(ratings[row]), str(price), str(back))
                cursor.execute(sql)
            try:
                if (price < forecast and price <= 5.5 and price >= 2.0 and forecast < 100.0 and forecast >= 4.0):
                    betFactor = forecast / price
                    if (betFactor > BestFactor):
                        BestRow =row
                        BestHorseID = horseID
                        BestMarketID = marketID
                        BestPrice = price
                        BestForecast = forecast
                        BestFactor = betFactor
            except:
                pass
        db.commit()

        if (BestHorseID != "None"):
            # bet half the forecast price, capped at the current price
            fAmount = BestForecast * 0.5
            if (fAmount > BestPrice):
                fAmount = BestPrice
            fAmount = fAmount * 100.0
            iAmount = int(fAmount)
            fAmount = float(iAmount) / 100.0


            #myprint ("Placing bet {} {} {} {} {}\n".format(str(BestRow),str(BestMarketID), str(BestHorseID), str(BestPrice), str(fAmount)))


        results,horses,markets,prices,backs,forecasts,forms,horsenames,races,ratings = HorseForm(SSOID,str(marketList[x]))

        del results
        del horses
        del markets
        del prices
        del backs
        del forecasts
        del forms
        del horsenames
        del races
        del ratings

    del cresults
    del marketList
    del venueList
    del timeList

    time.sleep(60)

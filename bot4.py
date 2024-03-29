import json
import datetime
import time
import urllib
import urllib.request
import urllib.error
import requests
import pandas as pd

my_app_key = ""
bet_url="https://api.betfair.com/exchange/betting/json-rpc/v1"

def getSSOID():
    my_username = ""
    my_password = ""

    payload = 'username=' + my_username + '&password=' + my_password
    headers = {'X-Application': my_app_key, 'Content-Type': 'application/x-www-form-urlencoded'}
    resp = requests.post('https://identitysso-cert.betfair.com/api/certlogin',data=payload,cert=('client-2048.crt','client-2048.key'),headers=headers)
    json_resp=resp.json()
    SSOID = json_resp['sessionToken']
    return SSOID



def CheckBet(SSOID,market):

    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}


    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listCurrentOrders"}'

    #print (user_req)
    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    #print (result)

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

    #print (user_req)

    placed,horse = CheckBet(SSOID,market)
    if (placed == "No"):
        req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
        response= urllib.request.urlopen(req)
        jsonResponse = response.read()
        pkg = jsonResponse.decode('utf-8')
        result = json.loads(pkg)
        #print (result)
    else:
        pass
        print ("You already have a bet in that market\n")



def HorseForm(SSOID,marketId):

    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(hours=12))
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

    #print (user_req)
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

            #print (price_req)
            req = urllib.request.Request(bet_url, data=price_req.encode('utf-8'), headers=headers)
            price_response= urllib.request.urlopen(req)
            price_jsonResponse = price_response.read()
            price_pkg = price_jsonResponse.decode('utf-8')
            price_result = json.loads(price_pkg)
            #print (price_result)

            #print (horsename)
            start_time = marketCatalogue[x]['marketStartTime']
            my_datetime = datetime.datetime.strptime(start_time, '%Y-%m-%dT%H:%M:%S.000Z')
            StartTime = my_datetime.strftime('%H:%M')
            venue = marketCatalogue[x]['event']['venue']
            try:
                price = float(price_result['result'][0]['runners'][0]['ex']['availableToLay'][0]['price'])
            except:
                price = "NR"
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
            venueList.append(venue)
            timeList.append(StartTime)
            ratingList.append(runnerRating)
            forecastList.append(forecast)

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

            Results = Results.append({'Horse Name':str(horsenameList[w]), 'Forecast':str(forecast), 'Form':str(formList[w]), 'Race':str(raceList[w]), 'Time':str(timeList[w]), 'Venue':str(venueList[w]), 'Rating':str(ratingList[w]), 'Odds':str(priceList[w]), 'Bet Placed':betPlaced, 'Bet Horse':betHorse }, ignore_index=True)

    return (Results,horseList,marketList,priceList,forecastList)

def getEvents(SSOID):
    event_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listEventTypes", "params": {"filter":{ }}, "id": 1}'
    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    req = requests.post(bet_url, data=event_req.encode('utf-8'), headers=headers)
    eventTypes = req.json()
    #print (eventTypes)

def getMarketCatalogue(SSOID):
    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(hours=12))
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

        betPlaced,horseid = CheckBet(SSOID,marketId)
        betHorse = "None"
        for ll in range(len(marketCatalogue[x]['runners'])):
            hname = marketCatalogue[x]['runners'][ll]['runnerName']
            sID = marketCatalogue[x]['runners'][ll]['selectionId']
            if (sID == horseid):
                betHorse = hname

        Results = Results.append({'Venue':str(venue), 'Start Time':str(StartTime), 'Horse':str(betHorse)}, ignore_index=True)

    return (Results,marketList,venueList,timeList)

SSOID = getSSOID()
#print (SSOID)
results,marketList,venueList,timeList = getMarketCatalogue(SSOID)
print ("\n")
print (results)

markets = []
prices = []
horses = []
forecasts = []

state = 1
finish = 0
while (finish == 0):
    print ("\n")
    if (state == 1):
        selMarket = input ("Select event ")
        try:
            row = int(selMarket)
            marketID = marketList[row]
            print ("{} {}".format(venueList[row], timeList[row]))
            results,horses,markets,prices,forecasts = HorseForm(SSOID,str(marketID))
            print ("\n")
            try:
                print (results)
            except:
                e = sys.exc_info()[0]
                print (e)
            state = 2
        except:
            finish = 1
    else:
        placeBets = input ("Place Bets (enter index) ")
        try:
            row = int(placeBets)
            horseID = horses[row]
            marketID = markets[row]
            price = prices[row]
            forecast = forecasts[row]
            betAmount = int((forecast / price) * 200.0)
            fAmount = float(betAmount) / 100.0
            if (fAmount > 4.0):
                fAmount = 4.0
            confirm = input ("Confirm bet (Enter y) ")
            if (confirm == "y" or confirm == "Y"):
                PlaceBet (SSOID, str(marketID), str(horseID), str(price), str(fAmount))
                results,horses,markets,prices,forecasts = HorseForm(SSOID,str(marketID))
                print (results)
            state = 1
        except:
            state = 1

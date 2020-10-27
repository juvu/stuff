import json
import datetime
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


def HorseForm(SSOID,BestOrWorst):
    Rating = float(0)
    Index = float(0)
    if (BestOrWorst == "Best"):
        FormRatingAvg = float(100)
    else:
        FormRatingAvg = float(0)
    FormRatingList = []
    horsename = []
    selectionID=[]

    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(hours=24))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    inplay = 'false' #still to run
    priceProjection = '["EX_BEST_OFFERS"]'#Best odds

    #Create an empty dataframe
    d = {'Horse Name': [], 'Form':[], 'Race': [], 'Time': [], 'Venue': [], 'Odds':[]}
    pd.set_option('display.max_columns', None)
    pd.set_option('display.max_colwidth', None)
    pd.set_option('expand_frame_repr', False)
    Results = pd.DataFrame(data=d)

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
    marketCatelogue = result['result']

    for x in range(len(marketCatelogue)):
        for w in range(len(marketCatelogue[x]['runners'])):
            runnerform = marketCatelogue[x]['runners'][w]['metadata']['FORM']
            if runnerform is None:
                runnerform = 'e'

            runnerformList = list(runnerform)

            for Entry in runnerformList:
                if Entry == 'R':#refusal to jump hurdle
                    Rating = float(Rating) + float(5)
                    Index = Index + 1
                elif Entry == 'e':#First Race
                    Rating = float(Rating) + float(10)
                    Index = Index + 1
                elif Entry == '0':#finished higher than 9th
                    Rating = float(Rating) + float(10)
                    Index = Index + 1
                elif Entry == 'F':#fell
                    Rating = float(Rating) + float(5)
                    Index = Index + 1
                elif Entry == 'U':#unseated rider
                    Rating = float(Rating) + float(3)
                    Index = Index + 1
                elif Entry == 'x':#horse has not started in a race for 3 months or more
                    Rating = float(Rating) + float(3)
                    Index = Index + 1
                elif Entry == 'C':#horse has won before at this same race distance and at this same track.
                    Rating = float(Rating) + float(.5)
                    Index = Index + 1
                elif Entry == 'B':#horse started favorite at it's last start, but it did not win
                    Rating = float(Rating) + float(3.5)
                    Index = Index + 1
                elif Entry == '/':#represents two seasons ago
                    Rating = float(Rating) + float(8)
                    Index = Index + 1
                elif Entry == '-':#represents one season ago
                    Rating = float(Rating) + float(4)
                    Index = Index + 1
                elif Entry == 'P':#pulled up by jockey
                    Rating = float(Rating) + float(4)
                    Index = Index + 1
                elif Entry == 'S':#what does this mean?
                    Rating = float(Rating) + float(4)
                    Index = Index + 1
                else:
                    Rating = float(Rating) + float(Entry)
                    Index = Index + 1

            rating = float(Rating)/float(Index)
            FormRatingList.append(rating)

        for t in range(len(FormRatingList)):
            if (BestOrWorst == "Best"):
                if FormRatingList[t] < FormRatingAvg:
                    FormRatingAvg = FormRatingList[t]
                    horsename.append(marketCatelogue[x]['runners'][t]['runnerName'])
                    selectionID.append(marketCatelogue[x]['runners'][t]['selectionId'])
            else:
                if FormRatingList[t] > FormRatingAvg:
                    FormRatingAvg = FormRatingList[t]
                    horsename.append(marketCatelogue[x]['runners'][t]['runnerName'])
                    selectionID.append(marketCatelogue[x]['runners'][t]['selectionId'])


        price_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listRunnerBook", "params": {"locale":"en", \
                    "marketId":"'+str(marketCatelogue[x]['marketId'])+'",\
                    "selectionId":"'+str(selectionID[-1])+'",\
                    "priceProjection":{"priceData":'+priceProjection+'},"orderProjection":"ALL"},"id":1}'

        req = urllib.request.Request(bet_url, data=price_req.encode('utf-8'), headers=headers)
        price_response= urllib.request.urlopen(req)
        price_jsonResponse = price_response.read()
        price_pkg = price_jsonResponse.decode('utf-8')
        price_result = json.loads(price_pkg)


        try:
            #print (horsename)
            start_time = marketCatelogue[x]['marketStartTime']
            venue = marketCatelogue[x]['event']['venue']
            Results = Results.append({'Horse Name':str(horsename[-1]), 'Form':str(int(FormRatingAvg)), 'Race':str(marketCatelogue[x]['marketName']), 'Time':str(start_time), 'Venue':str(venue), 'Odds':str(price_result['result'][0]['runners'][0]['ex']['availableToBack'][0]['price'])}, ignore_index=True)
        except:
            pass
            #print ("Got an error")

        Rating = float(0)
        Index = float(0)
        if (BestOrWorst == "Best"):
            FormRatingAvg = float(100)
        else:
            FormRatingAvg = float(0)
        FormRatingList.clear()
        horsename.clear()
        selectionID.clear

    return Results

def getEvents(SSOID):
    event_req = '{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listEventTypes", "params": {"filter":{ }}, "id": 1}'
    headers = {'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    req = requests.post(bet_url, data=event_req.encode('utf-8'), headers=headers)
    eventTypes = req.json()
    #print (eventTypes)

def getMarketCatelogue(SSOID):
    eventTypeID = '["7"]' #ID for Horse Racing
    countryCode= '["GB","IE"]' #Country Codes. Betfair use Alpha-2 Codes under ISO 3166-1
    marketTypeCode='["WIN"]' #Market Type
    MarketStartTime= datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ') #Event Start and End times
    MarketEndTime = (datetime.datetime.now() + datetime.timedelta(hours=24))
    MarketEndTime = MarketEndTime.strftime('%Y-%m-%dT%H:%M:%SZ')
    maxResults = str(1000)
    sortType = 'FIRST_TO_START' #Sorts the Output
    Metadata = 'RUNNER_METADATA' #Provides metadata
    inplay = 'false' #still to run

    user_req='{"jsonrpc": "2.0", "method": "SportsAPING/v1.0/listMarketCatalogue",\
           "params": {"filter":{"eventTypeIds":'+eventTypeID+',"marketTypeCodes":'+marketTypeCode+',\
           "inPlayOnly":'+inplay+', "marketCountries":'+countryCode+',  \
           "marketStartTime":{"from":"'+MarketStartTime+'", "to":"'+MarketEndTime+'"}},\
           "sort":"'+sortType+'", "maxResults":"'+maxResults+'", "marketProjection":["'+Metadata+'"]}, "id": 1}'

    req = urllib.request.Request(bet_url, data=user_req.encode('utf-8'), headers=headers)
    response= urllib.request.urlopen(req)
    jsonResponse = response.read()
    pkg = jsonResponse.decode('utf-8')
    result = json.loads(pkg)
    marketCatelogue = result['result']

    #print (marketCatelogue)

SSOID = getSSOID()
print (SSOID)
results = HorseForm(SSOID,"Best")
print ("BEST\n")
print (results)
print ("\n\nWORST\n")
results = HorseForm(SSOID,"Worst")
print (results)


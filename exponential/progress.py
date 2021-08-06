import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import sys
from dateutil.parser import parse

def myprint(x):
    print(x)
    sys.stdout.flush()


lay = pd.read_csv('./PlaceLayMulti.csv',sep=',')
back = pd.read_csv('./PlaceBackMulti.csv',sep=',')
single = pd.read_csv('./PlaceLaySingle.csv',sep=',')
focus = pd.read_csv('./BetFocus.csv',sep=',')

allresults = []
layresults = []
backresults = []
singleresults = []
focusresults = []

asdftime = parse("4th Jul 2021 10:30")
asdf = asdftime.date()
x = lay.loc[:,"Date Time"]
y = lay.loc[:,"Total"]
p = lay.loc[:,"Prof"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    allitem = {"Date":asdf,"Prof":p[count]}
    allresults.append(allitem)
    if count == 0:
        asdf = date
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        layresults.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
layresults.append(dictitem)

asdf = asdftime.date()
x = back.loc[:,"Date Time"]
y = back.loc[:,"Total"]
p = back.loc[:,"Prof"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    allitem = {"Date":asdf,"Prof":p[count]}
    allresults.append(allitem)
    if count == 0:
        asdf = date
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        backresults.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
backresults.append(dictitem)

asdf = asdftime.date()
x = single.loc[:,"Date Time"]
y = single.loc[:,"Total"]
p = single.loc[:,"Prof"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    allitem = {"Date":asdf,"Prof":p[count]}
    allresults.append(allitem)
    if count == 0:
        asdf = date
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        singleresults.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
singleresults.append(dictitem)

asdf = asdftime.date()
x = focus.loc[:,"Date Time"]
y = focus.loc[:,"Total"]
p = focus.loc[:,"Prof"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    allitem = {"Date":asdf,"Prof":p[count]}
    allresults.append(allitem)
    if count == 0:
        asdf = date
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        focusresults.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
focusresults.append(dictitem)
sresults = sorted(allresults, key=lambda k: k['Date'])

fresults = []

asdf = asdftime.date()
total = 0
count = 0
for x in sresults:
    date = x['Date']
    if count == 0:
        asdf = date
    if date == asdf:
        total += x['Prof']
    else:
        dictitem = {"Date":asdf,"Total":total}
        fresults.append(dictitem)
        total += x['Prof']
        asdf = date

    count = count+1

dictitem = {"Date":date,"Total":total}
fresults.append(dictitem)

dates = []
totals = []
for x in fresults:
    dates.append(x['Date'])
    totals.append(x['Total'])

ldates = []
ltotals = []
for x in layresults:
    ldates.append(x['Date'])
    ltotals.append(x['Total'])

bdates = []
btotals = []
for x in backresults:
    bdates.append(x['Date'])
    btotals.append(x['Total'])

sdates = []
stotals = []
for x in singleresults:
    sdates.append(x['Date'])
    stotals.append(x['Total'])

fdates = []
ftotals = []
for x in focusresults:
    fdates.append(x['Date'])
    ftotals.append(x['Total'])

plt.figure(figsize=(10,6))
plt.grid(True)
plt.xlabel('Date')
plt.ylabel('Total')
plt.plot(dates,totals,label="Total")
plt.plot(ldates,ltotals,label="MultiLay")
plt.plot(bdates,btotals,label="MultiBack")
plt.plot(sdates,stotals,label="LaySingle")
plt.plot(fdates,ftotals,label="BetFocus")
plt.title('Exponential Bet Progress')
plt.legend()
plt.show()



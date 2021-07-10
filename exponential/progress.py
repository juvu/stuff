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

results = []
layresults = []
backresults = []

asdftime = parse("3rd Jul 2021 20:30")
asdf = asdftime.date()
x = lay.loc[:,"Date Time"]
y = lay.loc[:,"Total"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        layresults.append(dictitem)
        results.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
layresults.append(dictitem)
results.append(dictitem)

asdftime = parse("3rd Jul 2021 20:30")
asdf = asdftime.date()
x = back.loc[:,"Date Time"]
y = back.loc[:,"Total"]
count = 0
lasttotal = 0
for z in x:
    datetime = parse(x[count])
    date = datetime.date()
    if date > asdf:
        dictitem = {"Date":asdf,"Total":lasttotal}
        backresults.append(dictitem)
        results.append(dictitem)
        asdf = date
    else:
        lasttotal = y[count]

    count = count+1

dictitem = {"Date":date,"Total":lasttotal}
backresults.append(dictitem)
results.append(dictitem)

sresults = sorted(results, key=lambda k: k['Date'])

fresults = []

asdftime = parse("3rd Jul 2021 20:30")
asdf = asdftime.date()
total = 0
for x in sresults:
    date = x['Date']
    if date == asdf:
        total += x['Total']
    else:
        dictitem = {"Date":asdf,"Total":total}
        fresults.append(dictitem)
        total = 0
        total += x['Total']
        asdf = date

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

plt.figure(figsize=(10,6))
plt.grid(True)
plt.xlabel('Date')
plt.ylabel('Total')
plt.plot(dates,totals,label="Total")
plt.plot(ldates,ltotals,label="MultiLay")
plt.plot(bdates,btotals,label="MultiBack")
plt.title('Exponential Bet Progress')
plt.legend()
plt.show()



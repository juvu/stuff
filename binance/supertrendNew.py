import ccxt
import websocket, json, talib, pprint
import config
import schedule
import pandas as pd
import sys
from binance.enums import *
from binance.client import Client


def myprint(x):
    print(x)
    sys.stdout.flush()

def writeTradeSettings(tradeSettings):
        f = open ("tradeSettings.txt", "w")
        for x in tradeSettings:
            f.write (x['name'])
            f.write (" ")
            f.write (x['tradeName'])
            f.write (" ")
            f.write (x['interval'])
            f.write (" ")
            f.write (str(x['amount']))
            f.write (" ")
            f.write (str(x['in_position']))
            f.write ("\n")
        f.close()

def readTradeSettings():
    tradeSettings = []
    try:
        f = open("tradeSettings.txt", "r")
        for line in f:
            x = line.rstrip()
            settings = []
            settings = x.split(" ")
            settings[3] = float(settings[3])
            if (settings[4] == 'False'):
                settings[4] = False
            elif (settings[4] == 'True'):
                settings[4] = True
            dictItem = {"name":settings[0], "tradeName":settings[1], "interval":settings[2], "amount":settings[3], "in_position":settings[4]}
            tradeSettings.append (dictItem)
        f.close()
    except:
        myprint ("No tradeSettings file")

    return tradeSettings


pd.set_option('display.max_columns', None)
pd.set_option('display.max_colwidth', None)
pd.set_option('expand_frame_repr', False)
pd.set_option('display.max_rows', None)


import warnings
warnings.filterwarnings('ignore')

import numpy as np
from datetime import datetime
import time

client = Client(config.BINANCE_API_KEY, config.BINANCE_SECRET_KEY)

def order(quantity, symbol, side, order_type):
    try:
        order = client.create_order(symbol=symbol,
                side=side,
                type=order_type,
                quantity=quantity)
        myprint("order_info")
        myprint (order)
    except Exception as e:
        myprint("There was a problem ordering - {}".format(e))
        return False

def on_open(ws):
    myprint("opened")


exchange = ccxt.binance({
    "apiKey": config.BINANCE_API_KEY,
    "secret": config.BINANCE_SECRET_KEY
})

def tr(data):
    data['previous_close'] = data['close'].shift(1)
    data['high-low'] = abs(data['high'] - data['low'])
    data['high-pc'] = abs(data['high'] - data['previous_close'])
    data['low-pc'] = abs(data['low'] - data['previous_close'])

    tr = data[['high-low', 'high-pc', 'low-pc']].max(axis=1)

    return tr

def atr(data, period):
    data['tr'] = tr(data)
    atr = data['tr'].rolling(period).mean()

    return atr

def supertrend(df, period=7, atr_multiplier=2.5):
    hl2 = (df['high'] + df['low']) / 2
    df['atr'] = atr(df, period)
    df['upperband'] = hl2 + (atr_multiplier * df['atr'])
    df['lowerband'] = hl2 - (atr_multiplier * df['atr'])
    df['in_uptrend'] = True

    for current in range(1, len(df.index)):
        previous = current - 1

        if df['close'][current] > df['upperband'][previous]:
            df['in_uptrend'][current] = True
        elif df['close'][current] < df['lowerband'][previous]:
            df['in_uptrend'][current] = False
        else:
            df['in_uptrend'][current] = df['in_uptrend'][previous]

            if df['in_uptrend'][current] and df['lowerband'][current] < df['lowerband'][previous]:
                df['lowerband'][current] = df['lowerband'][previous]

            if not df['in_uptrend'][current] and df['upperband'][current] > df['upperband'][previous]:
                df['upperband'][current] = df['upperband'][previous]
        
    return df


def check_buy_sell_signals(df, x):

    myprint("checking for buy and sell signals {} {}".format(x['name'], x['interval']))
    myprint(df.tail(5))
    last_row_index = len(df.index) - 1
    previous_row_index = last_row_index - 1

    if not df['in_uptrend'][previous_row_index] and df['in_uptrend'][last_row_index]:
        myprint("changed to uptrend, buy")
        if not x['in_position']:
            order_succeeded = order(x['amount'], x['tradeName'], SIDE_BUY, ORDER_TYPE_MARKET)
            x['in_position'] = True
        else:
            myprint("already in position, nothing to do")
    
    if df['in_uptrend'][previous_row_index] and not df['in_uptrend'][last_row_index]:
        if x['in_position']:
            myprint("changed to downtrend, sell")
            order_succeeded = order(x['amount'], x['tradeName'], SIDE_SELL, ORDER_TYPE_MARKET)
            x['in_position'] = False
        else:
            myprint("You aren't in position, nothing to sell")

def run_bot():
    myprint(f"Fetching new bars for {datetime.now().isoformat()}")
    tradeSettings = readTradeSettings()
    for x in tradeSettings:
        bars = exchange.fetch_ohlcv(x['name'], timeframe=x['interval'], limit=100)
        df = pd.DataFrame(bars[:-1], columns=['timestamp', 'open', 'high', 'low', 'close', 'volume'])
        df['timestamp'] = pd.to_datetime(df['timestamp'], unit='ms')

        supertrend_data = supertrend(df)
    
        check_buy_sell_signals(supertrend_data, x)

    writeTradeSettings(tradeSettings)


#schedule.every(30).seconds.do(run_bot)


while True:
    run_bot()
    time.sleep(30)


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
import telebot
import readAccount

#Note telebot installed with pip install pyTelegramBotAPI

account_url="https://api.betfair.com/exchange/account/json-rpc/v1"
acceptStr = "application/json"


def myprint(x):
    print(x)
    sys.stdout.flush()

def getSSOID():
    payload = 'username=' + my_username + '&password=' + my_password
    headers = {'X-Application': my_app_key, 'Content-Type': 'application/x-www-form-urlencoded'}
    resp = requests.post('https://identitysso-cert.betfair.com/api/certlogin',data=payload,cert=('client-2048.crt','client-2048.key'),headers=headers)
    json_resp=resp.json()
    #myprint (json_resp)
    SSOID = json_resp['sessionToken']
    return SSOID

def keepAlive(SSOID):
    headers = {'Accept': acceptStr, 'X-Application': my_app_key, 'X-Authentication': SSOID, 'content-type': 'application/json'}
    resp = requests.post('https://identitysso-cert.betfair.com/api/keepAlive',headers=headers)
    json_resp=resp.json()
    myprint (json_resp['status'])

def status(SSOID):
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
    resultStr = "available {:.2f}\nexposure {:.2f}\ntotal {:.2f}".format(available, exposure, total) 
    return (resultStr)

# main starts here

my_app_key,my_username,my_password,antsBetfairBot = readAccount.readAccount()

pid = os.getpid()
print (pid)

f = open ("accountPID.txt","w")
f.write(str(pid))
f.close()

bot = telebot.TeleBot(antsBetfairBot)

@bot.message_handler(commands=['start', 'status'])
def send_welcome(message):
    SSOID = getSSOID()
    accountStatus = status(SSOID)
    bot.reply_to(message, accountStatus)

@bot.message_handler(func=lambda message: True)
def echo_all(message):
    bot.reply_to(message, message.text)

bot.polling()

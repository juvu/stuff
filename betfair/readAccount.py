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


def myprint(x):
    print(x)
    sys.stdout.flush()

def readAccount():
    appKey = "asdf"
    username = "asdf"
    pwd = "asdf"
    bot = "asdf"

    f = open ("accountDetails.txt","r")

    for line in f:
        data = line.split('::')
        tok  = data[0].strip()
        value = data[1].strip()
        if (tok == "my_app_key"):
            appKey = value
        elif (tok == "my_username"):
            username = value
        elif (tok == "my_password"):
            pwd = value
        elif (tok == "BetfairBot"):
            bot = value

    f.close()

    return (appKey, username, pwd, bot)


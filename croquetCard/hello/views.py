from django.shortcuts import render
from django.http import HttpResponse
import requests

# Import modules for CGI handling 
import cgi, cgitb

import subprocess
from subprocess import Popen, PIPE

from .models import Greeting

# Create your views here.
def index(request):
    # return HttpResponse('Hello from Python!')
    return render(request, "index.html")

def cash(request):
    # Returns a dictionary in which the values are lists

    print ("asdf")
    params = request.GET
    randomSeed = params['RandomSeed']
    inflation = params['InflationMean']
    spend = params['YearSpend']
    outStr = '<body>' \
              '<center><TABLE COLS=3\n border=3>'  \
              '<TR>' \
              '<TD><center> RandomSeed <center>' \
              '<TD><center> Inflation <center>' \
              '<TD><center> Spend <center>' \
              '<TR>' \
              '<TD><center>' + randomSeed + '<center>' \
              '<TD><center>' + inflation + '<center>' \
              '<TD><center>' + spend + '<center>' \
              '</body>' 

    return HttpResponse(outStr)


#def index(request):
#    r = requests.get('http://httpbin.org/status/418')
#    print(r.text)
#    return HttpResponse('<pre>' + r.text + '</pre>')


def db(request):

    greeting = Greeting()
    greeting.save()

    greetings = Greeting.objects.all()

    return render(request, "db.html", {"greetings": greetings})

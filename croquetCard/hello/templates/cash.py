#!/usr/bin/env/python3

# Import modules for CGI handling 
import cgi, cgitb
import subprocess
from subprocess import Popen, PIPE

#cgi.test()

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
randomSeed = form.getvalue('RandomSeed')
inherit = form.getvalue('Inheritance')
rent = form.getvalue('Rent')
inheritYear = form.getvalue('InheritYear')
YearSpend = form.getvalue('YearSpend')
ZurichYear = form.getvalue('ZurichYear')
Zurich25 = form.getvalue('Zurich25')
if (Zurich25 == "on"):
    Zurich25 = "1"
else:
    Zurich25 = "0"
PruYear = form.getvalue('PruYear')
Pru25 = form.getvalue('Pru25')
if (Pru25 == "on"):
    Pru25 = "1"
else:
    Pru25 = "0"
FerrantiYear = form.getvalue('FerrantiYear')
SimonYear = form.getvalue('SimonYear')
InflationMean = form.getvalue('InflationMean')
InflationSD = form.getvalue('InflationSD')
InvestMean = form.getvalue('InvestMean')
InvestSD = form.getvalue('InvestSD')
CashMean = form.getvalue('CashMean')
CashSD = form.getvalue('CashSD')
SpendDecrease = form.getvalue('SpendDecrease')
FirstYear = form.getvalue('FirstYear')
ZRP = form.getvalue('ZRP')
Zurich  = form.getvalue('Zurich')
Pru  = form.getvalue('Pru')
cash  = form.getvalue('Cash')
FerrantiAmount  = form.getvalue('FerrantiAmount')
SimonAmount  = form.getvalue('SimonAmount')
StatePension  = form.getvalue('StatePension')
TaxAllowance  = form.getvalue('TaxAllowance')
RealTerms  = form.getvalue('RealTerms')
if (RealTerms == "on"):
    RealTerms = "1"
else:
    RealTerms = "0"
Perf  = form.getvalue('Perf')
YearPercent  = form.getvalue('YearSpendFactor')
BaseSpending  = form.getvalue('BaseSpending')
Index  = form.getvalue('Index')



print ("Content-type: text/html")
print ("")
print ("<html>")
print ("<head>")
print ("<title>Asset forecast</title>")
print ("</head>")
print ("<body>")
print("cgi-bin/cash {} {}".format(randomSeed,rent))
#print ("<h2>{}</h2>".format(cmd))
print ("</body>")
print ("</html>")




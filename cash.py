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


#randomSeed = "1234" 
#inherit = "100"
#rent = "0"
#inheritYear = "2025"
#YearSpend = "20000"
#ZurichYear = "2025"
#Zurich25 = "on"
#if (Zurich25 == "on"):
    #Zurich25 = "1"
#else:
    #Zurich25 = "0"
#PruYear = "2025"
#Pru25 = "on"
#if (Pru25 == "on"):
    #Pru25 = "1"
#else:
    #Pru25 = "0"
#FerrantiYear = "2025"
#SimonYear = "2025"
#InflationMean = "2"
#InflationSD = "1"
#InvestMean = "2"
#InvestSD = "1"
#CashMean = "2"
#CashSD = "1"
#SpendDecrease = "0"
#FirstYear = "2021" 
#ZRP = "200"
#Zurich  = "200"
#Pru  = "200"
#cash  = "200"
#FerrantiAmount  = "0"
#SimonAmount  = "0"
#StatePension  = "7500"
#TaxAllowance  = "12500"
#RealTerms  = "on"
#if (RealTerms == "on"):
    #RealTerms = "1"
#else:
    #RealTerms = "0"
#Perf  = "Average"
#YearPercent  = "0"
#BaseSpending  = "20000"
#Index  = "0"

print ("Content-type: text/html")
print ("")




print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("Hello - Second CGI Program")
#process = Popen(['cat', 'asdf'], stdout=PIPE, stderr=PIPE)
#stdout, stderr = process.communicate()
#thefile = "asdf"
#bytestr = subprocess.check_output(['cat', thefile])
bytestr = subprocess.check_output(['cgi-bin/cash',randomSeed,rent,inherit,inheritYear,YearSpend,ZurichYear,Zurich25,PruYear,Pru25,FerrantiYear,SimonYear,InflationMean,InflationSD,InvestMean,InvestSD,CashMean,CashSD,SpendDecrease,FirstYear,ZRP,Zurich,Pru,cash,FerrantiAmount,SimonAmount,StatePension,TaxAllowance,RealTerms,Perf,YearPercent,BaseSpending,Index])
asdf = bytestr.decode('ascii')
print (asdf)
#print ("<h2>{}</h2>".format(cmd))
print ("</body>")
print ("</html>")

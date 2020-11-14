import psutil
import os

try:
    f = open ("watchdogPID.txt","r")
    pid = int(f.read())
    print (pid)
    f.close()
    for proc in psutil.process_iter():
        process = proc.pid ;
        if (process == pid):
            proc.terminate()
except:
    pass

try:
    f = open ("collectDogPID.txt","r")
    pid = int(f.read())
    print (pid)
    f.close()
    for proc in psutil.process_iter():
        process = proc.pid ;
        if (process == pid):
            proc.terminate()
except:
    pass

try:
    f = open ("newBotPID.txt","r")
    pid = int(f.read())
    print (pid)
    f.close()
    for proc in psutil.process_iter():
        process = proc.pid ;
        if (process == pid):
            proc.terminate()
except:
    pass

try:
    f = open ("collectBotPID.txt","r")
    pid = int(f.read())
    print (pid)
    f.close()
    for proc in psutil.process_iter():
        process = proc.pid ;
        if (process == pid):
            proc.terminate()
except:
    pass


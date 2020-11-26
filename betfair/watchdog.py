import psutil
import os
import datetime
import time
import sys

def myprint(x):
    print(x)
    sys.stdout.flush()

pid = os.getpid()
f = open ("watchdogPID.txt","w")
f.write(str(pid))
f.close()

while(1):
    timenow = int(datetime.datetime.now().strftime('%H'))
    myprint (timenow)
    if (timenow > 21 or timenow < 10):
        time.sleep(60)
    else:
        found = 0
        try:
            f = open ("newBotPID.txt","r")
            pid = int(f.read())
            myprint (pid)
            f.close()
     
            for proc in psutil.process_iter():
                process = proc.pid
                if (process == pid):
                    found = 1
     
        except:
            pass
    
        if (found == 0):
            os.system('nohup python3 newBot.py > nohupBot.out')

        time.sleep(60)



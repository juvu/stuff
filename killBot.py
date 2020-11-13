import psutil
import os

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


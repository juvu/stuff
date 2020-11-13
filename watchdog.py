import psutil
import os

pid = os.getpid()
f = open ("watchdogPID.txt","w")
f.write(str(pid))
f.close()

while(1):
    found = 0
    try:
        f = open ("newBotPID.txt","r")
        pid = int(f.read())
        print (pid)
        f.close()
    
        for proc in psutil.process_iter():
            process = proc.pid
            if (process == pid):
                found = 1
    
    except:
        pass

    if (found == 0):
        #os.system('rm newBotPID.txt')
        os.system('nohup python3 newBot.py')

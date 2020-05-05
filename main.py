import sys, os, signal, datetime as dt

def sig_handler(signum, frame):
    print('Signal handler called with signal', signum, frame)

def time_handler(signum, frame):
    print(dt.datetime.now().second)

try:
    signal.signal(signal.SIGALRM, time_handler) #timer
    signal.signal(52, sig_handler)
    #check_timer = Timer(1, time_handler)
    #check_timer.start()

    signal.setitimer(signal.ITIMER_REAL, 1, 1)
    
    while(1==1):
        x = 1
except:
    print ("shiii", sys.exc_info())


import pyrtsignal as r
import os

def sighandler(s,v):
    print("sighandler for",s,v)

r.set_sighandler(0, xxx)
r.set_sighandler(31, xxx)

r.test_sighander(0,10)

pid = os.getpid()


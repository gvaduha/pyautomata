#! /usr/bin/env python3

import pyrtsignal as r
import os, time

def sighandler(s,v):
    print("sighandler for",s,v)

r.set_sighandler(0, sighandler)
#r.set_sighandler(31, sighandler)

#print("--- local tests ---")
#r.test_sighandler(0,10)

print("--- signal tests ---")
pid = os.getpid()
print("pid:",pid)
print("sigfun:",sighandler)
#r.send_signal(pid, 0, 11)

time.sleep(120)
print("test finished")

#! /usr/bin/env python3
import os, time, threading
from detector import run_detector

def start_detector():
    global cmdpipe, respipe
    cmdr, cmdw = os.pipe() #cmd pipe
    resr, resw = os.pipe() #res pipe
    pid = os.fork()
    if pid:
        print("controller waiting for detector")
        os.close(cmdr)
        os.close(resw)
        cmdpipe = os.fdopen(cmdw, 'w')
        respipe = os.fdopen(resr)
        x = respipe.read()
        print("controller go further", x)
    else:
        #child
        os.close(cmdw)
        os.close(resr)
        #run_detector(cmdr, resw) #out is in for child

def start_processing():
    cmdpipe.write("1")


def main():
    start_detector()
    start_processing()

if __name__ == "__main__":
    main()

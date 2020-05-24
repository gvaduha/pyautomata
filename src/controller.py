#! /usr/bin/env python3
import os, time, threading
from detector import run_detector

def start_detector():
    global inpipe, outpipe
    ir, iw = os.pipe() #in pipe
    ur, uw = os.pipe() #out pipe
    pid = os.fork()
    if pid:
        print("controller waiting for detector")
        os.close(iw)
        os.close(ur)
        inpipe = os.fdopen(ir)
        outpipe = os.fdopen(uw, 'w')
        inpipe.read()
        print("controller go further")
    else:
        #child
        os.close(ir)
        os.close(uw)
        run_detector(ur, iw) #out is in for child

def start_processing():
    outpipe.write(1)


def main():
    start_detector()
    start_processing()

if __name__ == "__main__":
    main()

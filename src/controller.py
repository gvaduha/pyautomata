#! /usr/bin/env python3
import os, time, threading
from detector import run_detector

global cmdpipe

def start_detector():
    cmdr, cmdw = os.pipe() #cmd pipe
    resr, resw = os.pipe() #res pipe
    os.set_inheritable(cmdr, True)
    os.set_inheritable(resw, True)
    pid = os.fork()
    if pid:
        print("controller waiting for detector")
        os.close(cmdr)
        os.close(resw)
        global cmdpipe, respipe
        cmdpipe = os.fdopen(cmdw, 'w')
        respipe = os.fdopen(resr)
        x = respipe.readline()
        print("controller go further")
    else:
        run_detector(cmdr, resw) #out is in for child
        exit(0)

def start_processing():
    cmdpipe.write("2\n")
    cmdpipe.flush()


def main():
    start_detector()
    start_processing()
    time.sleep(5)

if __name__ == "__main__":
    main()

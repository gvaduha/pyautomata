#! /usr/bin/env python3
import os, time, threading
from detector import run_detector

def start_detector():
    global inpipe, outpipe
    r, w = os.pipe()
    pid = os.fork()
    if pid:
        print("controller waiting for detector")
        inpipe = os.fdopen(r)
        outpipe = os.fdopen(w, 'w')
        inpipe.read()
        print("controller go further")
    else:
        #child
        run_detector(r, w)

def start_processing():
    outpipe.write(1)


def main():
    start_detector()
    start_processing()

if __name__ == "__main__":
    main()

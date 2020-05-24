#! /usr/bin/env python3
import os, time, threading
from detector import run_detector

def start_detector():
    r, w = os.pipe()
    pid = os.fork()
    if pid:
        os.close(w)
        rp = os.fdopen(r)
        rp.read()
        rp.close()
        print("controller go further")
    else:
        #child
        os.close(r)
        run_detector(w)


def main():
    start_detector()
    #start_processing()

if __name__ == "__main__":
    main()

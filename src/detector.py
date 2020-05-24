#! /usr/bin/env python3
import os, time

def run_detector(fd):
    print("starting detector")
    time.sleep(1)
    print("detector started!")
    wpipe = os.fdopen(fd, 'w')
    wpipe.write("x")
    wpipe.close()

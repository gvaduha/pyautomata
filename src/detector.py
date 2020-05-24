#! /usr/bin/env python3
import os, time

def run_detector(rfd, wfd):
    global inpipe, outpipe
    print("starting detector")
    initialize_detection_module()
    inpipe = os.fdopen(rfd)
    outpipe = os.fdopen(wfd, 'w')
    outpipe.write("x")
    print("detector started!")
    run_detection_module()

def initialize_detection_module():
    time.sleep(1)

def run_detection_module():
    global inpipe, outpipe
    while(1):
        num = inpipe.read()
        print("process item #", num)
        detect(int(num))
        print("completed process item #", num)
        outpipe.write(num)

def detect(n):
    time.sleep(0.5)

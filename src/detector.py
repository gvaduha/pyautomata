#! /usr/bin/env python3
import os, time

#cmdpipe = 0
#respipe = 0

def run_detector(cmd_fd, res_fd):
    global cmdpipe, respipe
    print("starting detector")
    initialize_detection_module()
    cmdpipe = os.fdopen(cmd_fd)
    respipe = os.fdopen(res_fd, 'w')
    respipe.write("x")
    print("detector started!")
    #run_detection_module()

def initialize_detection_module():
    time.sleep(1)

def run_detection_module():
    global cmdpipe, respipe
    while(1):
        num = cmdpipe.read()
        print("process item #", num)
        detect(int(num))
        print("completed process item #", num)
        respipe.write(num)

def detect(n):
    time.sleep(0.5)

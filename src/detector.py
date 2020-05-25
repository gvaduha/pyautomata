#! /usr/bin/env python3
import sys, os, time
from protocol import ResultType, status_text, response_text

def run_detector(cmd_fd = None, res_fd = None):
    if cmd_fd == None and res_fd == None:
        global cmdpipe, respipe
        cmdpipe = sys.stdin
        respipe = sys.stdout
    else:
        init_pipes(cmd_fd, res_fd)

    initialize_detection_module()
    respipe.write(status_text(ResultType.STARTED))
    respipe.flush()
    run_detection_module()

def init_pipes(cmd_fd, res_fd):
    global cmdpipe, respipe
    initialize_detection_module()
    cmdpipe = os.fdopen(cmd_fd)
    respipe = os.fdopen(res_fd, 'w')

def initialize_detection_module():
    time.sleep(1)

def run_detection_module():
    global cmdpipe, respipe
    while(1):
        num = cmdpipe.readline()
        print("process item #", num)
        detect(int(num))
        print("completed process item #", num)
        respipe.write(num)

def detect(n):
    time.sleep(0.5)


if __name__ == "__main__":
    run_detector()

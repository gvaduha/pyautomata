#! /usr/bin/env python3
import sys, os, time
from protocol import ResultType, status_text, response_text
from detector import initialize_detection_module, detect
from logger import logger

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

def run_detection_module():
    while(1):
        num = cmdpipe.readline()
        num = int(num)
        logger.debug("detector process item #{0}".format(num))
        detect(num)
        logger.debug("detector completed process item #{0}".format(num))
        resmsg = response_text(ResultType.GOODITEM, num)
        respipe.write(resmsg)
        respipe.flush()


if __name__ == "__main__":
    run_detector()

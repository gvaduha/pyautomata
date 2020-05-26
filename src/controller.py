#! /usr/bin/env python3
import sys, os, time, threading
from detector_controller import run_detector
from logger import logger

# globals
global cmdpipe, respipe
global current_item, processed_item

# start detector as separate process to communicate with
# cmdpipe - sending signals to process item
# respipe - receiving results
def start_detector():
    cmdr, cmdw = os.pipe() #cmd pipe
    resr, resw = os.pipe() #res pipe
    os.set_inheritable(cmdr, True)
    os.set_inheritable(resw, True)
    pid = os.fork()
    if pid:
        logger.info("controller waiting for detector")
        os.close(cmdr)
        os.close(resw)
        global cmdpipe, respipe
        cmdpipe = os.fdopen(cmdw, 'w')
        respipe = os.fdopen(resr)
        x = respipe.readline()
        logger.info("detector started ready to process")
    else:
        run_detector(cmdr, resw) #out is in for child
        exit(0)

# sending alarm to external system if critical error ocured
def send_illegal_state_alarm(msg):
    logger.error("ILLEGAL SIGNAL: {0}".format(msg))
    pass ############ TODO: Implement function

# thread to process incoming signals to start new item processing
def newitem_signal_thread():
    global current_item
    current_item = 0 ############ TODO: Implement function
    try:
        while(1):
            logger.debug("new signal #{0}".format(current_item))
            cmdpipe.write("{0}\n".format(current_item))
            current_item = current_item + 1
            cmdpipe.flush()
            time.sleep(1)
    except:
        send_illegal_state_alarm("new item thread: {0}".format(sys.exc_info()))

# thread to process result of item processed with newitem_signal_thread
def result_processing_thread():
    try:
        while(1):
            logger.debug("result processing waiting for next item")
            res = respipe.readline()
            logger.debug("result received #{0}".format(res))
    except:
        send_illegal_state_alarm("result processing thread: {0}".format(sys.exc_info()))

# check if new items coming faster than it processing
def check_tardiness_thread():
    if (current_item - processed_item > 2): # we dont want to lock as it is very relative measure
        send_illegal_state_alarm("detector working slower than incoming items")

# unhandled exceptions hook
def unhandled_excepthook(type, value, traceback):
        send_illegal_state_alarm("unhandled error: {0} {1} \n{2}]".format(type, value, traceback))
        exit(88)

# main
def main():
    sys.excepthook = unhandled_excepthook
    start_detector()
    thr_newsig = threading.Thread(target=newitem_signal_thread)
    thr_newsig.start()
    thr_resproc = threading.Thread(target=result_processing_thread)
    thr_resproc.start()

    thr_newsig.join()
    #thr_resproc.join()

    #thr_chktard = threading.Thread(target=check_tardiness_thread)
    #thr_chktard.start()
    #thr_chktard.join()

def check_tardiness_thread():
    if (current_item - processed_item > 2): # we dont want to lock as it is very relative measure
        send_illegal_state_alarm("detector working slower than incoming items")
    exit(1) # we have never exit!


if __name__ == "__main__":
    main()

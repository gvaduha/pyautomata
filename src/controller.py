#! /usr/bin/env python3
import sys, os, time, threading
from detector_controller import run_detector

#global cmdpipe
global current_item, processed_item

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

def send_illegal_state_alarm(msg):
    pass ############ TODO: Implement function

def newitem_signal_thread():
    global current_item
    current_item = 0 ############ TODO: Implement function
    try:
        while(1):
            cmdpipe.write("{0}\n".format(current_item))
            current_item = current_item + 1
            cmdpipe.flush()
            time.sleep(0.6)
    except:
        send_illegal_state_alarm("new item thread: {0}".format(sys.exc_info()))

def check_tardiness_thread():
    if (current_item - processed_item > 2):
        send_illegal_state_alarm("detector working slower than incoming items")

def result_processing_thread():
    try:
        while(1):
            res = respipe.readline()
            print("result received", res)
    except:
        send_illegal_state_alarm("result processing thread: {0}".format(sys.exc_info()))

def unhandled_excepthook(type, value, traceback):
        send_illegal_state_alarm("unhandled error: {0} {1} \n{2}]".format(type, value, traceback))
        exit(88)


def main():
    sys.excepthook = unhandled_excepthook
    start_detector()
    newitem_signal_thread()
    exit(1) # we have never exit!

if __name__ == "__main__":
    main()

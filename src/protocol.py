from enum import Enum

# result pipe format and types
# {'t': ResultType, ...}
class ResultType:#(Enum):
    STARTED = 1     # {}
    GOODITEM = 2    # {..., 'n': ITEM#}
    BADITEM = 3     # {..., 'n': ITEM#}

def status_text(command):
    return "{{'t':{0}\}}\n".format(command)

def response_text(state, number):
    return "{{'t':{0},'n':{1}}}\n".format(state, number)

import logging

logger = logging.getLogger('AutomatA')
logger.setLevel(logging.DEBUG)
#loghdlr = logging.handlers.RotatingFileHandler('automation.log', 'a', 100*1024*1024, 5)
loghdlr = logging.StreamHandler()
loghdlr.setLevel(logging.DEBUG)
logfmtr = logging.Formatter('%(asctime)s | %(name)s | %(process)d | %(thread)d | %(levelname)s | %(message)s')
loghdlr.setFormatter(logfmtr)
logger.addHandler(loghdlr)


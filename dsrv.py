import sys, logging

logger = logging.getLogger(__name__)
logging.basicConfig(filename='example.log', filemode='w', level=logging.DEBUG)

def handle_unhandled_exception(exc_type, exc_value, exc_traceback):
    logger.critical("Unhandled exception", exc_info=(exc_type, exc_value, exc_traceback))


sys.excepthook = handle_unhandled_exception



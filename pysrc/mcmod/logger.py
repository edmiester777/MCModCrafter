from mcmodinternal import Logger as l

##
# A wrapper around the internal logger built by the frontend application.
# Use this class to interact with the log.
class Logger(l):
    ##
    # Initialize a new logger with a name.
    #
    # @param Name of your plugin.
    def __init__(self, name = "unknown"):
        l.__init__(self, name)
    
    ##
    # Write a debug message to logger.
    #
    # @param text Message to log.
    def debug(self, text):
        l.debug(self, text)

    ##
    # Write an info message to logger.
    #
    # @param text Message to log.
    def info(self, text):
        l.info(self, text)

    ##
    # Write a warning message to logger.
    #
    # @param text Message to log.
    def warn(self, text):
        l.warn(self, text)
    
    ##
    # Write an error message to logger.
    #
    # @param text Message to log.
    def error(self, text):
        l.error(self, text)

    ##
    # Write a fatal message to logger.
    #
    # @param text Message to log.
    def fatal(self, text):
        l.fatal(self, text)
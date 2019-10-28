"""
Minimal mock for the ISSofaPython native C++ module, especially to provide it to
pylint that cannot introspect the real module.
"""

def loadPlugin(name):
    pass

def msg_fatal(msg):
    pass

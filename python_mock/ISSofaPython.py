"""
Minimal mock for the ISSofaPython native C++ module, especially to provide it to
pylint that cannot introspect the real module.
"""

def loadPlugin(name):
    pass

def msg_fatal(msg):
    pass

class _NodeMock(object):
    pass

def createRootNode(name):
    return _NodeMock()

def initializeGraph(node):
    pass

def cleanupGraph(node):
    pass

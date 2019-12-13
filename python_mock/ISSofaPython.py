"""
Minimal mock for the ISSofaPython native C++ module, especially to provide it to
pylint that cannot introspect the real module.
"""

def loadPlugin(name):
    pass

def msg_fatal(msg):
    pass

class _BaseContextMock(object):
    dt = None
    createObject = \
    createChild = \
    addObject = \
    removeObject = \
    getObject = \
    searchUp = \
    searchAllUp = \
    searchLocal = \
    searchAllLocal = \
    searchParents = \
    searchAllParents = \
    searchAllDown = \
    searchRoot = \
    searchAllRoot = \
    getObjects = \
    getDt = \
    getTime = \
        lambda *args, **kwargs: None

def createRootNode(name):
    return _BaseContextMock()

def getRoot():
    return _BaseContextMock()

def getNode(path):
    return _BaseContextMock()

def getObject(path):
    return _BaseContextMock()

def initializeGraph(node):
    pass

def cleanupGraph(node):
    pass

def step(node, dt):
    pass

def addDataPath(path):
    pass

class BaseObject(object):
    pass

class PythonController(object):
    def addCallback(self, evt, cb):
        pass

class PythonEvent(object):
    pass

class AnimateBeginEvent(object):
    def __init__(self, _):
        pass

class GNode(object):
    pass

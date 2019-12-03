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

def initializeGraph(node):
    pass

def cleanupGraph(node):
    pass

def step(node, dt):
    pass

def addDataPath(path):
    pass

import ISSofaPython as Sofa


class MyObject(Sofa.BaseObject):
    def __init__(self, name):
        Sofa.BaseObject.__init__(self)
        self.setName(name)

    def init(self):
        print(self.getName() + ": initialized")


def createScene(root):
    rootObject = MyObject("rootObject")
    root.addObject(rootObject)

    # the overriden init method of the MyObject type will not be called later
    # during the graph init since exiting here makes the rootObject instance to go
    # out of scope.
    return root

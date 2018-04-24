import ISSofaPython as Sofa

class MyObject(Sofa.BaseObject):
    def __init__(self,name):
        Sofa.BaseObject.__init__(self)
        self.setName(name)
    
    def init(self):
        print self.getName() + ": initialized"


rootObject = MyObject("rootObject")
root = Sofa.createRootNode("root")
root.addObject(rootObject)

childObject = MyObject("childObject")
child = root.createChild("child")
child.addObject(childObject)

child.addObject(MyObject("temporary python, will only call BaseObject::init"))

Sofa.initializeGraph(root)

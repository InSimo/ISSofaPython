import ISSofaPython as Sofa

class MyEvent(Sofa.PythonEvent):
    def __init__(self):
        Sofa.PythonEvent.__init__(self)

class MyObject(Sofa.PythonController):
    def __init__(self,name):
        Sofa.PythonController.__init__(self)
        self.setName(name)
        
    def callback(self, e ):
        print self.getClass().className + "::handleEvent("+ e.getClass().fullTypeName + " " + e.getClassName() +")"
    
    def init(self):
        print self.getName() + ": initialized"

def createScene(root):
    controller = MyObject("Controller")
    # because we add a callback to a member function of controller,
    # it also increases the ref count to controller
    controller.addCallback(MyEvent(), controller.callback) 

    root.addObject(controller)

    # controller does not go out of scope, so the overriden init method is indeed called later
    # when the graph is initialised.
    return root

import ISSofaPython as Sofa
import inspect

class MyEvent(Sofa.PythonEvent):
    def __init__(self):
        Sofa.PythonEvent.__init__(self)
    
class MyOtherEvent(Sofa.PythonEvent):
    def __init__(self):
        Sofa.PythonEvent.__init__(self)

class MyController(Sofa.PythonController):
    def __init__(self):
        super(MyController,self).__init__()
    
    def callback(self, e ):
        print self.getClass().className + "::handleEvent("+ e.getClass().fullTypeName + " " + e.getClassName() +")"

root = Sofa.createRootNode("root")
controller = MyController()
controller.setName("MyController")
root.addObject(controller)

controller.addCallback(MyEvent(), controller.callback)
controller.addCallback(MyOtherEvent(),controller.callback)

event      = MyEvent()
otherEvent = MyOtherEvent()

controller.handleEvent(event)
controller.handleEvent(otherEvent)

# either that of call controller.cleanup() to make sure that the controller instance is destroyed properly.
Sofa.cleanupGraph(root) 
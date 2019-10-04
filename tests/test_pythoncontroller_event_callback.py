import ISSofaPython as Sofa
import inspect

class MyEvent(Sofa.PythonEvent):
    def __init__(self):
        Sofa.PythonEvent.__init__(self)
        self.foo = "bar"
    
    def get_message(self):
        return self.foo
    
class MyOtherEvent(Sofa.PythonEvent):
    def __init__(self):
        Sofa.PythonEvent.__init__(self)
        self.bar = "foo"
    
    def get_message(self):
        return self.bar

class MyController(Sofa.PythonController):
    def __init__(self):
        super(MyController,self).__init__()
        self.foobar = "foobar"

    def callback(self, e ):
        print self.getClass().className + "::handleEvent("+ e.getClass().fullTypeName + " " + e.getClassName() +")"
        print self.foobar + " received " + e.get_message()

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
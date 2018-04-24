= README

ISSofaPython contains 
- a python module which defines bindings for the `SofaCore` library and `SofaSimulationTree` library. The source code for the python module is in the `module` folder
- a dedicated sofa plugin which contains a specific scene loader that can be used to load directly python scene files from the main application

It serves as a replacement for the SofaPython plugin. It uses the [[ https://pybind11.readthedocs.io/en/stable/intro.html | pybind11 library ]] to expose c++ types in Python.

== TL;DR

Write your python scene directly from the interpreter

```lang=python

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

Sofa.initializeGraph(root)

Sofa.step(root,0.02) # advance 0.02 seconds in time

Sofa.cleanupGraph(root)
```

Or write it so that it can be loaded from `runSofa` provided the `ISSofaPythonPlugin` is loaded

```lang=python
import ISSofaPython as Sofa

def createScene(root):
    rootObject = Sofa.BaseObject()
    rootObject.setName("rootObject")
    root.addObject(rootObject)

    childObject = Sofa.BaseObject()
    childObject.setName("childObject")
    child = root.createChild("child")
    child.addObject(childObject)

    child.addObject(Sofa.BaseObject() )
    return root
```

== Main differences compared to SofaPython 

From a user perspective several aspects differ from SofaPython

=== Data as a dynamic attributes of a component

Contrary to SofaPython, object `Data` are bound as dynamic attributes only when created from the `ObjectFactory`.

This works:
```lang=python
import ISSofaPython as Sofa

Sofa.loadPlugin("myPlugin")
node = Sofa.createRootNode("root")
obj  = node.createObject("MyObject", name="myObject")
print obj.name.value # output "myObject"
```

Assuming a binding for `MyObject` type has been implemented in the `ISSofaPython` module, the following does not work:
```lang=python
import ISSofaPython as Sofa

obj  = Sofa.MyObject()
print obj.name.value # throws Attribute Error python exception: object has no attribute 'name'
```

=== Accessing Data value

In SofaPython accessing the value stored in a `Data` result in typing something like
```lang=python 
obj.printLog = False
```

In ISSofaPython it requires typing
```lang=python
obj.printLog.value = False
```

This change is however more natural when dealing with other part of the `Data` API
For example setting the parent of a `Data` in SofaPython requires to write:
```lang=python
obj.findData('printLog').setParent(parentData)
```
However in ISSofaPython since it is really the `Data` that is bound as an attribute 
this works
```lang=python
obj.printLog.setParent(parentData)
```

=== Setting the value of a Data which wraps a vector/array type 

`ISSofaPython` uses more advanced reflection mechanisms for `Data` types compared to `SofaPython`.
For example to assign a python object to a Data which internally represents a `vector< Vec3d >`, a similar structure must also exist in the python object.
The syntax is currently permissive as shown in the following example:
```lang=python
import ISSofaPython as Sofa
#...

root = Sofa.createRootNode("root")
mobj = root.createObject("MechanicalObject", template="Vec3d", mame="MO" )
# The following gives a very different result compared to SofaPython 
mobj.position.value = [ 0,1,0 ]
mobj.position.value  # prints  [(0.0, 0.0, 0.0), (1.0, 1.0, 1.0), (0.0, 0.0, 0.0)] 

# This is the correct way to assign to a `Data` which stores a `vector<Vec3d>` with ISSofaPython
mobj.position.value = [ (0,0,0), (1,0,0) ]

# This works as well
mobj.position.value = [ [0,0,0], [1,0,0] ]

```

== Limitations

=== Accessing the value of a Data

This version implements only partially bindings for the reflection mechanism of `Data`.

=== Subclassing a C++ pybind11 class in Python

Currently inheriting from a Sofa type has only practical limited support in terms of virtual method override, as explained in the comments of `tests/test_pythonsceneloader_inheritbaseobject.py` and `tests/test_pythonsceneloader_inherit_controller.py`.
One way this limitation could be alleviated would be to have the python Sofa graph also manage the lifetime of the instances of the python object that compose it.
For reference, this limitation has also been reported  [[https://github.com/pybind/pybind11/issues/1145 | here (issue 1145)]]  and also [[https://github.com/pybind/pybind11/issues/1333 | here (issue 1333)]] on the pybind11 github.
So this particular limitation could be well be removed in future versions of pybind11, but it is also likely that a feasible workaround can be implemented in the specific case of lifetime management of python objects in a Sofa graph instance.
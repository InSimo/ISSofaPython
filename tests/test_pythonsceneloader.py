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


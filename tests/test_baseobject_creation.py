import ISSofaPython as Sofa


node   = Sofa.GNode("root")
object = Sofa.BaseObject()
object.setName("object")
node.addObject(object)

assert(object.getContext() == node)
assert(node.getObject("object") == object)

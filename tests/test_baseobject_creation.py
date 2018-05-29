import ISSofaPython as Sofa


node   = Sofa.createRootNode("root")
object = Sofa.BaseObject()
object.setName("object")
node.addObject(object)

assert(object.getContext() == node)
assert(node.getObject("object") == object)
assert(node.getObject("@object") == object)
assert(node.getObject("/object") == object)
assert(node.getObject("@/object") == object)
assert(node.getObject("idonotexist") == None)
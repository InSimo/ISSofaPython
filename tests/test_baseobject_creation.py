import ISSofaPython as Sofa


node   = Sofa.createRootNode("root")
object = Sofa.BaseObject()

# test data value assignement
object.name.value = "object_name"
assert(object.getName() == "object_name")
node.addObject(object)
assert(object.getContext() == node)

assert( hasattr(object,"name") == True ) 

# test dynamic attribute creation
object.foo = "bar"
assert( hasattr(object,"foo") == True )

object.setName("object")
assert(object.getName() == "object")

# test object lookup
assert(node.getObject("object") == object)
assert(node.getObject("@object") == object)
assert(node.getObject("/object") == object)
assert(node.getObject("@/object") == object)
assert(node.getObject("idonotexist") == None)
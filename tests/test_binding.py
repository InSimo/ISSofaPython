import ISSofaPython as Sofa


for plugin in ('ISSofaPythonPlugin'):
    try:
        Sofa.loadPlugin(plugin)
    except (RuntimeError, ValueError) as err:
        print('Failed to load the %r plugin: %s', plugin, err)

def createScene(root_node):
    mo_0 = root_node.createObject("MechanicalObject")
    print(type(mo_0))
    print(mo_0.findData("rest_position").value)
    child_node = root_node.createChild("child")
    mo_1 = child_node.createObject("MechanicalObject")
    mapping = child_node.createObject("IdentityMapping")
    print(isinstance(mapping, Sofa.Base))
    print(isinstance(mapping, Sofa.BaseObject))
    # print(isinstance(mapping, Sofa.BaseMapping))
    print(type(mapping))
    print(mapping.getClass().className)
    print(mapping.findData("mapForces").value)
    print(mapping.mapForces.value)
    print(mapping.reinit)
    mapping.reinit()
    mo_0.cleanup()
    print("mapping reinit")

    controller = Sofa.PythonController()
    print("controller created")
    print(type(controller))
    child_node.addObject(controller)


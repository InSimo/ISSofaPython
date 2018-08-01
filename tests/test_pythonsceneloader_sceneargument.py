# If called through runSofa with a scene option, the object added to the scene will be named after this option
# If called directly with the python interpreter, the object added to scene will be named after the first command-line argument

import sys
import ISSofaPython as Sofa

objName = "defaultName"


def parseSceneArguments(args):
    print("Scene args: " + str(args))
    if len(args) > 0:
        global objName
        objName = args[0]


def createScene(root):
    parseSceneArguments(sys.argv[1:])
    rootObject = Sofa.BaseObject()
    rootObject.setName(objName)
    root.addObject(rootObject)

    return root


if __name__ == "__main__":
    createScene(Sofa.GNode())

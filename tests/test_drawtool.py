import ISSofaPython as Sofa


class MyController(Sofa.PythonController):
    def __init__(self):
        Sofa.PythonController.__init__(self)

    def draw(self, vparams):
        vparams.drawTool.setLightingEnabled(False)

        vparams.drawTool.drawPoints([[2, 0, 0], [2, 1, 0]], 10., [1, 0, 0, 1])
        vparams.drawTool.drawLines([[2, 0, 0], [2, 1, 0]], 1., [0, 1, 0, 1])
        vparams.drawTool.drawFrame([-1, 0, 0], [0, 0, 0, 1], [0.3, 0.2, 0.1])
        vparams.drawTool.drawArrow([0, 1, 0], [0, 2, 0], 0.05, [0.5, 0.5, 0.5, 1])

        vparams.drawTool.setLightingEnabled(True)

        vparams.drawTool.drawTriangles([[1, -1, 0], [1, -2, 0], [1.5, -1.5, 0], [1, -1, 0], [1, -2, 0], [0.5, -0.5, 0]], [0.5, 0, 1, 1])
        vparams.drawTool.drawCylinder([-1, -1, 0], [-2, -2, 0], 0.3, [1, 1, 1, 1])
        vparams.drawTool.drawArrow([0, 3, 0], [0, 4, 0], 0.05, [0.5, 0.5, 0.5, 1], 4)
        vparams.drawTool.drawSphere([1, 0, 0], 0.1)
        vparams.drawTool.drawBoundingBox([-5, -5, -5], [5, 5, 5])
        vparams.drawTool.writeOverlayText(0, 0, 24, [1, 1, 1, 1], "Hello")


controller = None


def createScene(root):
    global controller  # prevent the python object from being destroyed when exiting createScene(), which would prevent the draw override from being called, resulting in nothing being drawn
    controller = MyController()
    controller.setName("MyController")
    root.addObject(controller)

    root.createObject('MechanicalObject', template='Vec3d', position="-3 0 0  3 0 0", showObject=False, showObjectScale=10)  # needed to correctly initialize the scene's bbox

    return root


if __name__ == "__main__":
    createScene(Sofa.GNode())

/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*            (c) 2006-2021 MGH, INRIA, USTL, UJF, CNRS, InSimo                *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include "DrawToolBinding.h"
#include "SofaSTLBinding.h"
#include <sofa/core/visual/DrawTool.h>

namespace sofa
{
namespace python
{

using sofa::core::visual::DrawTool;
using Vec4f = DrawTool::Vec4f;
using Vec3f = DrawTool::Vec3f;
using Vector3 = DrawTool::Vector3;
using Vec3i = DrawTool::Vec3i;
using Vec2i = DrawTool::Vec2i;
using Quaternion = DrawTool::Quaternion;


void initBindingDrawTool(pybind11::module& m)
{
    pybind11::class_<DrawTool>(m, "DrawTool")

        // Primitive rendering methods
        .def("drawPoints", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, float, const Vec4f>(&DrawTool::drawPoints))
        .def("drawPoints", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, float, const sofa::helper::vector<Vec4f>>(&DrawTool::drawPoints))

        .def("drawLines", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, float, const Vec4f>(&DrawTool::drawLines))
        .def("drawLines", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vec2i>&, float, const Vec4f>(&DrawTool::drawLines))

        .def("drawTriangles", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const Vec4f>(&DrawTool::drawTriangles))
        .def("drawTriangles", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const Vector3, const Vec4f>(&DrawTool::drawTriangles))
        .def("drawTriangles", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vec3i>&, const sofa::helper::vector<Vector3>&, const Vec4f>(&DrawTool::drawTriangles))
        .def("drawTriangles", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vec4f>&>(&DrawTool::drawTriangles))
        .def("drawTriangleStrip", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vector3>&, const Vec4f>(&DrawTool::drawTriangleStrip))
        .def("drawTriangleFan", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<Vector3>&, const Vec4f>(&DrawTool::drawTriangleFan))

        .def("drawFrame", pybind11::overload_cast<const Vector3&, const Quaternion&, const Vec3f&>(&DrawTool::drawFrame))
        .def("drawFrame", pybind11::overload_cast<const Vector3&, const Quaternion&, const Vec3f&, const Vec4f&>(&DrawTool::drawFrame))

        .def("drawSpheres", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, const sofa::helper::vector<float>&, const Vec4f>(&DrawTool::drawSpheres))
        .def("drawSpheres", pybind11::overload_cast<const sofa::helper::vector<Vector3>&, float, const Vec4f>(&DrawTool::drawSpheres))

        .def("drawCone", &DrawTool::drawCone, pybind11::arg("p1"), pybind11::arg("p2"), pybind11::arg("radius1"), pybind11::arg("radius2"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawCube", &DrawTool::drawCube, pybind11::arg("radius"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawCylinder", &DrawTool::drawCylinder, pybind11::arg("p1"), pybind11::arg("p2"), pybind11::arg("radius"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawCapsule", &DrawTool::drawCapsule, pybind11::arg("p1"), pybind11::arg("p2"), pybind11::arg("radius"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawArrow", &DrawTool::drawArrow, pybind11::arg("p1"), pybind11::arg("p2"), pybind11::arg("radius"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawPlus", &DrawTool::drawPlus, pybind11::arg("radius"), pybind11::arg("colour"), pybind11::arg("subd")=16)

        .def("drawPoint", pybind11::overload_cast<const Vector3&, const Vec4f&>(&DrawTool::drawPoint))
        .def("drawPoint", pybind11::overload_cast<const Vector3&, const Vector3&, const Vec4f&>(&DrawTool::drawPoint))

        .def("drawTriangle", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&, const Vector3&>(&DrawTool::drawTriangle))
        .def("drawTriangle", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vec4f&>(&DrawTool::drawTriangle))
        .def("drawTriangle", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vec4f&, const Vec4f&, const Vec4f&>(&DrawTool::drawTriangle))
        .def("drawTriangle", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vector3&, const Vector3&, const Vec4f&, const Vec4f&, const Vec4f&>(&DrawTool::drawTriangle))

        .def("drawQuad", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&,const Vector3&, const Vector3&>(&DrawTool::drawQuad))
        .def("drawQuad", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vec4f&>(&DrawTool::drawQuad))
        .def("drawQuad", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vec4f&, const Vec4f&, const Vec4f&, const Vec4f&>(&DrawTool::drawQuad))
        .def("drawQuad", pybind11::overload_cast<const Vector3&,const Vector3&,const Vector3&,const Vector3&, const Vector3&, const Vector3&, const Vector3&, const Vector3&, const Vec4f&, const Vec4f&, const Vec4f&, const Vec4f&>(&DrawTool::drawQuad))

        .def("drawSphere", &DrawTool::drawSphere)

        .def("drawBoundingBox", &DrawTool::drawBoundingBox)

        // Transformation methods
        .def("pushMatrix", &DrawTool::pushMatrix)
        .def("popMatrix", &DrawTool::popMatrix)
        .def("multMatrix", &DrawTool::multMatrix)
        .def("scale", &DrawTool::scale)

        // Drawing style methods
        .def("setMaterial", &DrawTool::setMaterial, pybind11::arg("colour"), pybind11::arg("name")=std::string())
        .def("resetMaterial", &DrawTool::resetMaterial, pybind11::arg("colour"), pybind11::arg("name")=std::string())
        .def("setPolygonMode", &DrawTool::setPolygonMode)
        .def("setLightingEnabled", &DrawTool::setLightingEnabled)
        .def("getColor", &DrawTool::getColor)

        // Overlay methods
        .def("writeOverlayText", &DrawTool::writeOverlayText)
        ;
}

}

}

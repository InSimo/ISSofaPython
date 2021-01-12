/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseMeshTopologyBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>

#include <sofa/core/topology/Topology.h>
#include <sofa/core/topology/BaseMeshTopology.h>

#include <pybind11/pybind11.h>

using namespace sofa;
using namespace sofa::core::topology;

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::BaseState;

namespace internal
{

pybind11::object indicesVectorToPyList(const sofa::helper::vector<BaseMeshTopology::index_type>& vector)
{
    pybind11::list list;
    for (unsigned int i = 0u; i < vector.size(); i++)
        list.append(vector[i]);
    return list;
}

pybind11::object getVerticesAroundVertex(BaseMeshTopology* obj, int pointId)
{
    return indicesVectorToPyList(obj->getVerticesAroundVertex(BaseMeshTopology::PointID(pointId)));
}

pybind11::object getEdgesAroundVertex(BaseMeshTopology* obj, int pointId)
{
    return indicesVectorToPyList(obj->getEdgesAroundVertex(BaseMeshTopology::PointID(pointId)));
}

pybind11::object getTrianglesAroundEdge(BaseMeshTopology* obj, int pointId)
{
    return indicesVectorToPyList(obj->getTrianglesAroundEdge(BaseMeshTopology::PointID(pointId)));
}

pybind11::object getTrianglesAroundVertex(BaseMeshTopology* obj, int pointId)
{
    return indicesVectorToPyList(obj->getTrianglesAroundVertex(BaseMeshTopology::PointID(pointId)));
}

pybind11::object getEdgesInTriangle(BaseMeshTopology* obj, int triangleId)
{
    sofa::helper::fixed_array<BaseMeshTopology::EdgeID,3> eint = obj->getEdgesInTriangle(BaseMeshTopology::TriangleID(triangleId));
    pybind11::list list;
    for (unsigned int i = 0u; i < 3; i++)
        list[i] = eint[i];
    return list;
}

}

void initBindingBaseMeshTopology(pybind11::module& m)
{
    pybind11::class_<BaseMeshTopology, Topology, 
                     PySofaBaseMeshTopology<BaseMeshTopology>,
                     sofa::sptr<BaseMeshTopology>>(m, "BaseMeshTopology", pybind11::multiple_inheritance())
        .def(pybind11::init<>() )
        .def("getNbEdges", &BaseMeshTopology::getNbEdges)
        .def("getNbTriangles", &BaseMeshTopology::getNbTriangles)
        .def("getNbQuads", &BaseMeshTopology::getNbQuads)
        .def("getNbTetrahedra", &BaseMeshTopology::getNbTetrahedra)
        .def("getNbHexahedra", &BaseMeshTopology::getNbHexahedra)
        .def("getVerticesAroundVertex", &internal::getVerticesAroundVertex)
        .def("getEdgesAroundVertex", &internal::getEdgesAroundVertex)
        .def("getTrianglesAroundVertex", &internal::getTrianglesAroundVertex)
        .def("getTrianglesAroundEdge", &internal::getTrianglesAroundEdge)
        .def("getEdgesInTriangle", &internal::getEdgesInTriangle)
        .def("getEdgesInTriangle", &internal::getEdgesInTriangle)

        .def("getEdges", &BaseMeshTopology::getEdges)
        .def("getTriangles", &BaseMeshTopology::getTriangles)
        .def("getQuads", &BaseMeshTopology::getQuads)
        .def("getTetrahedra", &BaseMeshTopology::getTetrahedra)
        .def("getHexahedra", &BaseMeshTopology::getHexahedra)
        .def("getTopologyType", &BaseMeshTopology::getTopologyType)
        ;

}

}

}

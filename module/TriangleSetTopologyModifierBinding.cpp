/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "TriangleSetTopologyModifierBinding.h"

#include <sofa/helper/vector.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/topology/Topology.h>
#include <SofaBaseTopology/PointSetTopologyModifier.h>
#include <SofaBaseTopology/TriangleSetTopologyModifier.h>

#include <pybind11/pybind11.h>

using namespace sofa;
using namespace sofa::core::topology;
using namespace sofa::component::topology;

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::BaseState;

namespace internal
{

Topology::Triangle parseTriangleTuple(pybind11::tuple& tuple)
{
    if (tuple.size() != 3)
    {
        throw pybind11::type_error("invalid triangle tuple: size should be 3");
    }
    Topology::Triangle T;
    T[0] = pybind11::cast<int>(tuple[0]);
    T[1] = pybind11::cast<int>(tuple[1]);
    T[2] = pybind11::cast<int>(tuple[2]);
    return T;
}

sofa::helper::vector<Topology::Triangle> parseTriangleList(pybind11::list& pyList)
{
    sofa::helper::vector< Topology::Triangle > triangles;

    if (!pyList.size())
        return triangles;

    bool isTwoDimensionsList = pybind11::isinstance<pybind11::list>(pyList[0]);
    if( isTwoDimensionsList )
    {
        throw pybind11::type_error("invalid list or triangles: should not be a list of list, but a list of tuple");
    }
    else
    {
        for (int i=0; i<pyList.size(); ++i)
        {
            pybind11::tuple tuple = pyList[i];
            triangles.push_back(parseTriangleTuple(tuple));
        }
    }

    return triangles;
}

// TODO: handle ancestors and coeffs args, if needed
void addTriangles(TriangleSetTopologyModifier* obj, pybind11::list pyTriangles)
{
    sofa::helper::vector<Topology::Triangle> triangles = parseTriangleList(pyTriangles);
    if(!triangles.empty())
    {
        obj->addTriangles(triangles);
    }
}

void removeTriangles(TriangleSetTopologyModifier* obj, pybind11::list pyTriangleIndices, bool removeIsolatedEdges, bool removeIsolatedPoints)
{
    sofa::helper::vector< unsigned int > triangleIndices;
    std::size_t nbTriangles = pyTriangleIndices.size();
    for(std::size_t i=0;i<nbTriangles;++i)
    {
        triangleIndices.push_back( pybind11::cast<unsigned int>(pyTriangleIndices[i]) );
    }
    obj->removeTriangles(triangleIndices, removeIsolatedEdges, removeIsolatedPoints);
}

}

void initBindingTriangleSetTopologyModifier(pybind11::module& m)
{
    pybind11::class_<TriangleSetTopologyModifier, PointSetTopologyModifier, 
                     sofa::sptr<TriangleSetTopologyModifier>>(m, "TriangleSetTopologyModifier", pybind11::multiple_inheritance())
        .def("addTriangles", &internal::addTriangles)
        .def("removeTriangles", &internal::removeTriangles)
        // addRemoveTriangles could be added if needed (see SofaPython for the implementation)
        ;
}

}

}

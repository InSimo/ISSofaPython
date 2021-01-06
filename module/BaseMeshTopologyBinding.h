/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEMESHTOPOLOGYBINDING_H
#define ISSOFA_PYTHON_BASEMESHTOPOLOGYBINDING_H

#include "BaseObjectBinding.h"

#include <pybind11/pybind11.h>

#include <sofa/core/topology/BaseMeshTopology.h>

namespace sofa
{

namespace python
{

void initBindingBaseMeshTopology(pybind11::module& m);


template< class TSofaObject >
class PySofaBaseMeshTopology : public PySofaBaseObject< TSofaObject >
{
public:

    typedef sofa::core::topology::BaseMeshTopology::SeqEdges SeqEdges;
    typedef sofa::core::topology::BaseMeshTopology::SeqTriangles SeqTriangles;
    typedef sofa::core::topology::BaseMeshTopology::SeqQuads SeqQuads;
    typedef sofa::core::topology::BaseMeshTopology::SeqTetrahedra SeqTetrahedra;
    typedef sofa::core::topology::BaseMeshTopology::SeqHexahedra SeqHexahedra;

    void isClassMacroUsed(TSofaObject* sofaObject) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, isClassMacroUsed, sofaObject);
    }

    const SeqEdges& getEdges() override 
    {
        PYBIND11_OVERLOAD_PURE(const SeqEdges&, TSofaObject, getEdges, );
    }

    const SeqTriangles& getTriangles() override 
    {
        PYBIND11_OVERLOAD_PURE(const SeqTriangles&, TSofaObject, getTriangles, );
    }

    const SeqQuads& getQuads() override 
    {
        PYBIND11_OVERLOAD_PURE(const SeqQuads&, TSofaObject, getQuads, );
    }

    const SeqTetrahedra& getTetrahedra() override 
    {
        PYBIND11_OVERLOAD_PURE(const SeqTetrahedra&, TSofaObject, getTetrahedra, );
    }

    const SeqHexahedra& getHexahedra() override 
    {
        PYBIND11_OVERLOAD_PURE(const SeqHexahedra&, TSofaObject, getHexahedra, );
    }

    sofa::core::topology::TopologyObjectType getTopologyType() const override 
    {
        PYBIND11_OVERLOAD_PURE(sofa::core::topology::TopologyObjectType, TSofaObject, getTopologyType, );
    }

    using PySofaBaseObject< TSofaObject >::PySofaBaseObject; // Inherit constructors

};
}

}


#endif // ISSOFA_PYTHON_BASEMESHTOPOLOGYBINDING_H

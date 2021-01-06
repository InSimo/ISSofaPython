/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PointSetTopologyModifierBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>

#include <sofa/core/topology/Topology.h>
#include <SofaBaseTopology/PointSetTopologyModifier.h>
#include <sofa/helper/vector.h>

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

void propagateEndingEvent(PointSetTopologyModifier* obj)
{
    obj->notifyEndingEvent();
    obj->propagateTopologicalChanges();
}

}

void initBindingPointSetTopologyModifier(pybind11::module& m)
{
    pybind11::class_<PointSetTopologyModifier, BaseObject,
                     sofa::sptr<PointSetTopologyModifier>>(m, "PointSetTopologyModifier")
        .def("propagateEndingEvent", &internal::propagateEndingEvent)
        ;
}

}

}

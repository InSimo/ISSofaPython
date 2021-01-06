/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "TopologyBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/topology/Topology.h>

#include <pybind11/pybind11.h>

using namespace sofa;
using namespace sofa::core::topology;

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;

namespace internal
{

}

void initBindingTopology(pybind11::module& m)
{
    pybind11::class_<Topology, BaseObject,
                     sofa::sptr<Topology>>(m, "Topology")
        ;
}

}

}

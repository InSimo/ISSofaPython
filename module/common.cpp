/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "common.h"

#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/Context.h>
#include <sofa/simulation/common/Node.h>

#include <sofa/core/topology/Topology.h>
#include <sofa/core/topology/BaseMeshTopology.h>

#include <sofa/core/BaseState.h>
#include <sofa/core/BaseMapping.h>
#include <sofa/core/CollisionModel.h>

#include <SofaBaseTopology/PointSetTopologyModifier.h>
#include <SofaBaseTopology/TriangleSetTopologyModifier.h>

namespace sofa
{

namespace python
{

pybind11::object getDerivedPyObject(sofa::core::objectmodel::Base* obj)
{
    if (auto derived = sofa::simulation::Node::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::core::objectmodel::Context::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::core::objectmodel::BaseContext::DynamicCast(obj))
        return pybind11::cast(derived);

    if (auto derived = sofa::core::topology::BaseMeshTopology::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::topology::Topology::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }

    if (auto derived = sofa::core::BaseState::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::BaseMapping::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::CollisionModel::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::component::topology::TriangleSetTopologyModifier::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::component::topology::PointSetTopologyModifier::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::objectmodel::BaseObject::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }

    return pybind11::cast(obj);
}

} // namespace python

} // namespace sofa

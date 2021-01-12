/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseMappingBinding.h"

#include "common.h"
#include "BaseBinding.h"
#include "BaseObjectBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/BaseState.h>
#include <pybind11/pybind11.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::BaseState;
using sofa::core::BaseMapping;

namespace internal
{

void processMapping(BaseMapping* mapping)
{
    mapping->apply(core::MechanicalParams::defaultInstance(), core::VecCoordId::position(), core::ConstVecCoordId::position());
    mapping->applyJ(core::MechanicalParams::defaultInstance(), core::VecDerivId::velocity(), core::ConstVecDerivId::velocity());
}

// // warning: not tested yet
// pybind11::object getFrom(BaseMapping* mapping)
// {
//     helper::vector<BaseState*> from = mapping->getFrom();
//     pybind11::list pyList;
//     for (BaseState* obj: from)
//     {
//         pyList.append(getDerivedPyObject(obj));
//     }
//     return pyList;
// }

// // warning: not tested yet
// pybind11::object getTo(BaseMapping* mapping)
// {
//     helper::vector<BaseState*> to = mapping->getTo();
//     pybind11::list pyList;
//     for (BaseState* obj: to)
//     {
//         pyList.append(getDerivedPyObject(obj));
//     }
//     return pyList;
// }

// // warning: not tested yet
// void setFrom(BaseMapping* mapping, BaseState* from)
// {
//     mapping->setFrom(from);
// }

// // warning: not tested yet
// void setTo(BaseMapping* mapping, BaseState* to)
// {
//     mapping->setTo(to);
// }

}

void initBindingBaseMapping(pybind11::module& m)
{
    pybind11::class_<BaseMapping, BaseObject,
                     PySofaBaseMapping<BaseMapping>, // trampoline "alias" class 
                     sofa::sptr<BaseMapping> >(m, "BaseMapping", pybind11::multiple_inheritance())
        .def(pybind11::init<>() )

        // .def("apply", &BaseMapping::apply)
        // .def("applyJ", &BaseMapping::applyJ)
        .def("processMapping", &internal::processMapping)
        // .def("getFrom", &internal::getFrom)
        // .def("getTo", &internal::getTo)
        // .def("setFrom", &internal::setFrom)
        // .def("setTo", &internal::setTo)

        .def("apply",&BaseMapping::apply)
        .def("applyJ",&BaseMapping::applyJ)
        .def("getFrom",&BaseMapping::getFrom)
        .def("setFrom",&BaseMapping::setFrom)
        .def("getTo",&BaseMapping::getTo)
        .def("setTo",&BaseMapping::setTo)
        .def("applyJT",pybind11::overload_cast<const MechanicalParams*, MultiVecDerivId, ConstMultiVecDerivId>(&BaseMapping::applyJT), "mechanics")
        .def("applyDJT",&BaseMapping::applyDJT)
        .def("applyJT",pybind11::overload_cast<const ConstraintParams*, MultiMatrixDerivId, ConstMultiMatrixDerivId>(&BaseMapping::applyJT), "constraints")
        .def("computeAccFromMapping",&BaseMapping::computeAccFromMapping)
        .def("getMechFrom",&BaseMapping::getMechFrom)
        .def("getMechTo",&BaseMapping::getMechTo)
        .def("disable",&BaseMapping::disable)
        ;

}

}

}

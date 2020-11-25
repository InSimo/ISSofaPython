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

#include "CollisionModelBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/CollisionModel.h>


namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::CollisionModel;

void initBindingCollisionModel(pybind11::module& m)
{
    pybind11::class_<CollisionModel, BaseObject,
                     PySofaCollisionModel<CollisionModel>, // "trampoline" alias class
                     sofa::sptr<CollisionModel>>(m, "CollisionModel", pybind11::multiple_inheritance())
        .def(pybind11::init<>() )
        .def("getContext", [](CollisionModel &self) { return self.getContext(); })
       // .def("getContext", pybind11::overload_cast<>(&CollisionModel::getContext, pybind11::const_), pybind11::return_value_policy::reference)
        .def("computeBoundingTree", &CollisionModel::computeBoundingTree)
        .def("setActive", &CollisionModel::setActive)
        ;
}

}

}

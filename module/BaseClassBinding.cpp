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

#include "BaseClassBinding.h"
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseClass;

void initBindingBaseClassInfo(pybind11::module& m)
{
    using BaseClassInfo  = sofa::defaulttype::BaseClassInfo;
    pybind11::class_<BaseClassInfo,
                    std::unique_ptr<BaseClassInfo, pybind11::nodelete> >(m, "BaseClassInfo")
        .def_readonly("fullTypeName", &BaseClassInfo::fullTypeName)
        .def_readonly("namespaceName", &BaseClassInfo::namespaceName)
        .def_readonly("className", &BaseClassInfo::className)
        .def_readonly("templateName", &BaseClassInfo::templateName)
        .def_readonly("shortName", &BaseClassInfo::shortName)
        .def_readonly("targetNames", &BaseClassInfo::targetNames)
        ;
}

void initBindingBaseClass(pybind11::module& m)
{
    return initBindingBaseRootClass<sofa::core::objectmodel::Base>(m, "BaseClass");
}

void initBindingEventClass(pybind11::module& m)
{
    return initBindingBaseRootClass<sofa::core::objectmodel::Event>(m, "EventClass");

}


}


}
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

#include "BaseLinkBinding.h"
#include <sofa/core/objectmodel/BaseLink.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/Base.h>



namespace sofa
{

namespace python
{

using sofa::core::objectmodel::BaseLink;


void baseLinkRead(BaseLink* link, const std::string& path)
{
    if (!link->read(path))
    {
        throw  std::invalid_argument(path.c_str());
    }
}


void initBindingBaseLink(pybind11::module& m)
{
    pybind11::class_<BaseLink>(m, "BaseLink")
        .def("setName", &BaseLink::setName)
        .def("getName", &BaseLink::getName)
        .def_property("name", &BaseLink::getName, &BaseLink::setName)
        .def("read", [](BaseLink* link, const std::string& path) { baseLinkRead(link, path);  })
        .def("getSize", &BaseLink::getSize)
        .def("getLinkedPath", &BaseLink::getLinkedPath, pybind11::arg("index")=0)
        .def("getLinkedBase", &BaseLink::getLinkedBase, pybind11::arg("index")=0)
        .def("getLinkedData", &BaseLink::getLinkedData, pybind11::arg("index")=0)
        ;
}


}

}
   
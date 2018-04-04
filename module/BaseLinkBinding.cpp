/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseLinkBinding.h"
#include <sofa/core/objectmodel/BaseLink.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/Base.h>

namespace sofa
{

namespace python
{

using sofa::core::objectmodel::BaseLink;

void initBindingBaseLink(pybind11::module& m)
{
    pybind11::class_<BaseLink>(m, "BaseLink")
        .def("setName", &BaseLink::setName)
        .def("getName", &BaseLink::getName)
        .def_property("name", &BaseLink::getName, &BaseLink::setName)
        .def("getSize", &BaseLink::getSize)
        .def("getLinkedPath", &BaseLink::getLinkedPath, pybind11::arg("index")=0)
        .def("getLinkedBase", &BaseLink::getLinkedBase, pybind11::arg("index")=0)
        .def("getLinkedData", &BaseLink::getLinkedData, pybind11::arg("index")=0)
        ;
}


}

}
   
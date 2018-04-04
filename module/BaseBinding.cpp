/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseBinding.h"
#include "SofaSTLBinding.h"
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;

void initBindingBase(pybind11::module& m)
{
    pybind11::class_<Base, PySofaBase<Base>, sofa::sptr<Base> > base(m, "Base");

    base.def("getName", &Base::getName, pybind11::return_value_policy::copy)
        .def("setName", pybind11::overload_cast<const std::string&>(&Base::setName))
        .def("setName", pybind11::overload_cast<const std::string&, int>(&Base::setName))
        .def("getClass", &Base::getClass, pybind11::return_value_policy::reference)
        .def("getSourceFileName", &Base::getSourceFileName)
        .def("getSourceFilePos", &Base::getSourceFilePos)
        .def("findData", &Base::findData, pybind11::return_value_policy::reference)
        .def("getDatas", &Base::getDataFields, pybind11::return_value_policy::reference)
        .def("findLink", &Base::findLink, pybind11::return_value_policy::reference)
        .def("getLinks", &Base::getLinks, pybind11::return_value_policy::reference)
        ;
}


}

}


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

pybind11::object bindDataAndLinks(sofa::sptr<core::objectmodel::BaseObject > obj)
{
    pybind11::object pyObj = pybind11::cast(obj);

    const auto dataVec = obj->getDataFields();

    for (auto data : dataVec)
    {
        if (!pybind11::hasattr(pyObj, data->getName().c_str()))
        {
            try
            {
                pybind11::object pyData = pybind11::cast(data);
                pyObj.attr(data->getName().c_str()) = pyData;
            }
            catch (pybind11::error_already_set&)
            {
                //We fall here when Data "value" attribute throws
                //which is the case when the DataTypeInfo is not supported by the binding
            }
        }
        else
        {
            pybind11::print(obj->getName(), ": Could not set Data attribute ", data->getName(), " since already set.");
        }
    }


    const auto linkVec = obj->getLinks();

    for (auto link : linkVec)
    {
        if (!pybind11::hasattr(pyObj, link->getName().c_str()))
        {
            try
            {
                pybind11::object pyLink = pybind11::cast(link);
                pyObj.attr(link->getName().c_str()) = pyLink;
            }
            catch (pybind11::error_already_set&)
            {
                pybind11::print(obj->getName(), ": unknown error when setting Link attribute ", link->getName());
            }
        }
        else
        {
            pybind11::print(obj->getName(), ": Could not set Link attribute ", link->getName(), " since already set.");
        }
    }

    return pyObj;
}

}

}


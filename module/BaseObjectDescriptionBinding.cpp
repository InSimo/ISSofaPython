/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseObjectDescriptionBinding.h"
#include <sofa/core/objectmodel/BaseClass.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseObjectDescription;
using sofa::core::objectmodel::BaseClass;


BaseObjectDescription createBaseObjectDescription(pybind11::args args, pybind11::kwargs kwargs)
{
    BaseObjectDescription objDescription;

    if (args)
    {
        const std::string type = std::string(pybind11::str(*args.begin()));
        objDescription.setAttribute(std::string("type"), type.c_str());
    }

    if (kwargs)
    {
        pybind11::dict dict(kwargs);

        for (auto item : dict)
        {
            std::string attr = std::string(pybind11::str(item.first));
            std::string val  = std::string(pybind11::str(item.second));
            objDescription.setAttribute(attr, val.c_str());
        }
    }

    return objDescription;
}

void initBindingBaseObjectDescription(pybind11::module& m)
{
    pybind11::class_<BaseObjectDescription>(m, "BaseObjectDescription")
        .def(pybind11::init(&createBaseObjectDescription));
}

}

}

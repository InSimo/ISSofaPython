/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseObjectDescriptionBinding.h"
#include "BaseDataBinding.h"
#include <sofa/core/objectmodel/BaseClass.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseObjectDescription;
using sofa::core::objectmodel::BaseClass;



bool isStringType(pybind11::handle h)
{
    return pybind11::isinstance<pybind11::str>(h);
}

bool isFundamentalType(pybind11::handle h)
{
    const bool isInt    = pybind11::isinstance<pybind11::int_>(h);
    const bool isScalar = pybind11::isinstance<pybind11::float_>(h);
    const bool isBool   = pybind11::isinstance<pybind11::bool_>(h);
    return isInt || isScalar || isBool;
}

bool isPythonTypeHandledByBaseObjectDescription(pybind11::handle h)
{
    return isFundamentalType(h) || isStringType(h);
}

std::string getDataStringRepresentation(pybind11::handle h)
{
    const bool isBool =  pybind11::isinstance<pybind11::bool_>(h);
    if (isBool)
    {
        bool b(pybind11::reinterpret_borrow<pybind11::bool_>(h));
        return b ? "1" : "0";
    }
    else
    {
        return std::string(pybind11::str(h));
    }
}

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
            if(isPythonTypeHandledByBaseObjectDescription(item.second) )
            {
                std::string val  = getDataStringRepresentation(item.second);
                objDescription.setAttribute(attr, val.c_str());
            }
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

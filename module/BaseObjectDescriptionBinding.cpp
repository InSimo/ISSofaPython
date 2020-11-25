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

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

#include "Exceptions.h"



namespace sofa
{

namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseLink;


namespace internal
{

std::string sofaExceptionPrefix(const Base* obj)
{
    if (obj)
    {
        return std::string("[" + obj->getName() + "(" + obj->getClassName() + ")]: ");
    }
    else
    {
        return std::string();
    }
}

std::string sofaExceptionPrefix(const BaseData* data)
{
    return sofaExceptionPrefix(data->getOwner()) + "Data \"" + data->getName() + "\" ";
}

std::string sofaExceptionPrefix(const BaseLink* link)
{
    return sofaExceptionPrefix(link->getOwnerBase()) + "Link \"" + link->getName() + "\" ";
}

}


template class TSofaAttributeError<sofa::core::objectmodel::Base, pybind11::attribute_error>;
template class TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::attribute_error>;
template class TSofaAttributeError<sofa::core::objectmodel::BaseLink, pybind11::attribute_error>;

template class TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::type_error>;

}

}
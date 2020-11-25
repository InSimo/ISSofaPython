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

#ifndef ISSOFA_PYTHON_EXCEPTIONS_H
#define ISSOFA_PYTHON_EXCEPTIONS_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/BaseLink.h>

namespace pybind11
{

// Python attribute error is not bound to a pybind11 exception in version 2.2.2  
// This will maybe change in future versions, in which case this declaration 
// will have to be removed
PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError)

}


namespace sofa
{

namespace python
{

namespace internal
{

std::string sofaExceptionPrefix(const sofa::core::objectmodel::Base* obj);

std::string sofaExceptionPrefix(const sofa::core::objectmodel::BaseData* data);

std::string sofaExceptionPrefix(const sofa::core::objectmodel::BaseLink* link);

}


///< Generic class which provides builtin exception message formatting for Sofa objects 
///< similar to the one you would get by using the object sout or serr output stream
template< class TSofaObject, class TPyException >
class TSofaAttributeError : public TPyException
{
public:
    TSofaAttributeError(const TSofaObject* obj, std::string message)
        :TPyException(internal::sofaExceptionPrefix(obj) + message)
    {

    }

    TSofaAttributeError(const TSofaObject* obj, const char* message)
        :TSofaAttributeError(obj, std::string(message))
    {

    }
};

using SofaAttributeError     = TSofaAttributeError<sofa::core::objectmodel::Base, pybind11::attribute_error>;
using SofaLinkAttributeError = TSofaAttributeError<sofa::core::objectmodel::BaseLink, pybind11::attribute_error>;

using SofaDataAttributeError = TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::attribute_error>;
using SofaDataIndexError     = TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::index_error>;
using SofaDataValueError     = TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::value_error>;
using SofaDataTypeError      = TSofaAttributeError<sofa::core::objectmodel::BaseData, pybind11::type_error>;

}


}


#endif // ISSOFA_PYTHON_EXCEPTIONS_H

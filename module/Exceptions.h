/*******************************************************************************
*          Private SOFA components, (c) 2019 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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

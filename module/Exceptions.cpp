/*******************************************************************************
*          Private SOFA components, (c) 2019 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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
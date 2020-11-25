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

#include "ObjectFactoryBinding.h"
#include "BaseObjectBinding.h"
#include "BaseDataBinding.h"
#include "BaseObjectDescriptionBinding.h"
#include "common.h"
#include "Exceptions.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

#include <pybind11/stl.h>
#include <pybind11/iostream.h>


// Required to return a vector of BaseClass* whose ownership must not be transferred to python
// See https ://github.com/pybind/pybind11/issues/637

using ListCasterBase = pybind11::detail::list_caster<std::vector<const sofa::core::objectmodel::BaseClass *>, const  sofa::core::objectmodel::BaseClass*>;

namespace pybind11 
{
namespace detail 
{

template<> struct type_caster<std::vector<const sofa::core::objectmodel::BaseClass *>> : ListCasterBase {
    static handle cast(const std::vector<const sofa::core::objectmodel::BaseClass *> &src, return_value_policy, handle parent) {
        return ListCasterBase::cast(src, return_value_policy::reference, parent);
    }
    static handle cast(const std::vector<const sofa::core::objectmodel::BaseClass *> *src, return_value_policy pol, handle parent) {
        return cast(*src, pol, parent);
    }
};

}

}

namespace sofa
{
namespace python
{

using sofa::core::ObjectFactory;
using sofa::core::objectmodel::BaseClass;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseObjectDescription;
using sofa::core::objectmodel::BaseData;

std::vector< const BaseClass* > getClass(ObjectFactory* factory, std::string className)
{
    std::vector< const BaseClass* > result;
    if (factory->hasCreator(className))
    {
        const auto& classEntry = factory->getEntry(className);
        for (auto kv : classEntry.creatorMap)
        {
            result.push_back(kv.second->getClass());
        }
    }

    return result;
}



pybind11::object createObject(ObjectFactory* factory, BaseContext* ctx, pybind11::args args, pybind11::kwargs kwargs)
{
    BaseObjectDescription objDescription = createBaseObjectDescription(args,kwargs);

    sofa::sptr<BaseObject > obj = factory->createObject(ctx, &objDescription);

    if (obj == nullptr)
    {
        throw pybind11::type_error("createObject failed with: " + std::string(pybind11::str(args))  + " " + std::string(pybind11::str(kwargs)) );
    }

    std::vector<std::string> unAccessedAttributes;
    objDescription.getAttributeList(unAccessedAttributes, false);

    if (!unAccessedAttributes.empty())
    {
        std::ostringstream oss;
        std::ostream_iterator<std::string::value_type> outIt(oss, ", ");

        for (std::size_t i=0;i<unAccessedAttributes.size();++i)
        {
            oss << unAccessedAttributes[i];
            const bool isLast = i == unAccessedAttributes.size() - 1;
            if (!isLast)
            {
                oss << ", ";
            }
        }
        throw SofaAttributeError(obj.get(), "unused attribute(s) : \"" + oss.str() + "\"");
    }

    pybind11::dict dict(kwargs);

    // process elements in the dict that are not taken care of by BaseObjectDescription
    for (auto item : dict)
    {
        std::string attr = std::string(pybind11::str(item.first));     

        if (!isPythonTypeHandledByBaseObjectDescription(item.second))
        {
            pybind11::object value = pybind11::reinterpret_borrow<pybind11::object>(item.second);

            // We allow passing python none objects only in the createObject method, in which case assignemt is simply ignored.
            if (value.is_none()) continue;

            // it can only be a data, since links are initialized from strings, which are taken care of by BaseObjectDescription.
            BaseData* data   = obj->findData(attr); 
            if (data)
            {
                // will throw invalid_argument if the python object cannot be converted to something compatible with the data type
                setDataValueFromPyObject(data, value);
            }
            else throw SofaAttributeError(obj.get(), "unused attribute: \"" + attr + "\"");
        }
    }

    setSourceLocation(obj.get());

    return getDerivedPyObject(obj.get());
}


void initBindingObjectFactory(pybind11::module& m)
{
    pybind11::class_<ObjectFactory, 
                    std::unique_ptr<ObjectFactory, pybind11::nodelete> // singleton class that must not be deleted by python 
                    >(m, "ObjectFactory")
        .def(pybind11::init([](void) { return ObjectFactory::getInstance(); }), pybind11::return_value_policy::reference)
        .def_static("getInstance", &ObjectFactory::getInstance, pybind11::return_value_policy::reference)
        .def("createObject", &ObjectFactory::createObject)
        .def("getClass", &getClass )
        ;
}


}


}


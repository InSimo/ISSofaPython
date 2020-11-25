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

#include "BaseBinding.h"
#include "SofaSTLBinding.h"
#include "BaseDataBinding.h"
#include "BaseObjectBinding.h"
#include "Exceptions.h"
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseObject.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseLink;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseNode;

namespace internal
{

pybind11::object getAttr(Base* self, const std::string& attr)
{
    std::string attribute = attr;
    sofa::core::objectmodel::Base::MapData::const_iterator findIt = self->getDataAliases().find(attr);
    if (findIt != self->getDataAliases().end())
    {
        attribute = findIt->second->getName();
    }

    if (BaseData* d = self->findData(attribute))
    {
        return pybind11::cast(d);
    }
    else if (BaseLink* l = self->findLink(attribute))
    {
        return pybind11::cast(l);
    }
    else
    {
        pybind11::object pySelf = pybind11::cast(self);
        return pySelf.attr("__getattribute__")(attribute.c_str());
    }
}


void setAttrErrorCheck(sofa::core::objectmodel::Base* self, const std::string& attr, pybind11::object value)
{
    if (BaseData* d = self->findData(attr))
    {
        if (value.is_none())
        {
            throw SofaDataAttributeError(d, "invalid assignment to python none");
        }

        const std::string correctSyntax = attr + ".value for deep copy or " + attr + ".setParent() for data link";
        try
        {

            value.cast<BaseData*>();
            throw SofaDataAttributeError(d, "no implicit BaseData->BaseData conversion, correct syntax: " + correctSyntax);

        }
        catch (pybind11::cast_error&) // not a BaseData
        {
            if (pybind11::isinstance<pybind11::str>(value))
            {
                throw SofaDataAttributeError(d, "no implicit string->BaseData conversion, correct syntax: " + correctSyntax);
            }

            throw SofaDataAttributeError(d, "no implicit conversion to BaseData, correct syntax: " + correctSyntax);
        }
    }
    else if (BaseLink* l = self->findLink(attr))
    {
        std::string path;

        if (value.is_none())
        {
            throw SofaLinkAttributeError(l, "invalid assignment to python none");
        }

        try
        {
            BaseObject* obj = value.cast<BaseObject*>();
            const std::string correctSyntax = attr + ".read(" + getPath(obj) + ")";
            throw SofaLinkAttributeError(l, "no implicit BaseObject->BaseLink conversion, correct syntax: " + correctSyntax);
        }
        catch (pybind11::cast_error&) // not a BaseObject
        {
            try
            {
                BaseData* data = value.cast<BaseData*>();
                const std::string correctSyntax = attr + ".read(" + data->getLinkPath() + ")";
                throw SofaLinkAttributeError(l, "no implicit BaseData->BaseLink conversion, correct syntax: " + correctSyntax);
            }
            catch (pybind11::cast_error&) // not a BaseData either
            {
                if (pybind11::isinstance<pybind11::str>(value))
                {
                    const std::string correctSyntax = attr + ".read()";
                    throw SofaLinkAttributeError(l, "no implicit string->BaseLinkconversion, correct syntax: " + correctSyntax);
                }
                else
                {
                    std::string correctSyntax = attr + ".read()";
                    throw SofaLinkAttributeError(l, "no implicit conversion to BaseLink, correct syntax: " + correctSyntax);
                }
            }
        }
    }
}



void setAttr(Base* self, const std::string& attr, pybind11::object value)
{
    // will throw an AttributeError if attempt is made to modifify a Data or a Link using implicit conversion
    setAttrErrorCheck(self, attr, value);
    // if not we can safely default to python object attribute dictionary
    pybind11::object pySelf = pybind11::cast(self);
    pySelf.attr("__dict__")[attr.c_str()] = value;
}

void delAttr(Base* self, const std::string& attr)
{
    if (BaseData* d = self->findData(attr))
    {
        throw SofaDataAttributeError(d, "cannot delete Data attribute: " + attr);
    }
    if (BaseLink* l = self->findLink(attr))
    {
        throw SofaLinkAttributeError(l, "cannot delete Link attribute: " + attr);
    }

    pybind11::object pySelf = pybind11::cast(self);
    pybind11::dict pyDict = pySelf.attr("__dict__");
    PyDict_DelItemString(pyDict.ptr(), attr.c_str());
}

}

void initBindingBase(pybind11::module& m)
{
    pybind11::class_<Base, PySofaBase<Base>, sofa::sptr<Base>> base(m, "Base", pybind11::dynamic_attr(), pybind11::multiple_inheritance() );

    base.def("getName", &Base::getName, pybind11::return_value_policy::copy)
        .def("setName", pybind11::overload_cast<const std::string&>(&Base::setName))
        .def("setName", pybind11::overload_cast<const std::string&, int>(&Base::setName))
        .def("getClass", &Base::getClass, pybind11::return_value_policy::reference)
        .def("addSourceFile", &Base::addSourceFile)
        .def("getNbSourceFiles", &Base::getNbSourceFiles)
        .def("getSourceFileName", &Base::getSourceFileName)
        .def("getSourceFilePos", &Base::getSourceFilePos)
        .def("findData", &Base::findData, pybind11::return_value_policy::reference)
        .def("findGlobalField", &Base::findGlobalField, pybind11::return_value_policy::reference)
        .def("getDatas", &Base::getDataFields, pybind11::return_value_policy::reference)
        .def("findLink", &Base::findLink, pybind11::return_value_policy::reference)
        .def("getLinks", &Base::getLinks, pybind11::return_value_policy::reference)
        .def("getWarnings", &Base::getWarnings, pybind11::return_value_policy::copy)
        .def("__getattr__", &internal::getAttr)
        .def("__setattr__", &internal::setAttr)
        .def("__delattr__", &internal::delAttr)
        ;
}

}

}


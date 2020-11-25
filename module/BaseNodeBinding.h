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

#ifndef ISSOFA_PYTHON_BASENODEBINDING_H
#define ISSOFA_PYTHON_BASENODEBINDING_H


#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include "BaseBinding.h"

namespace sofa
{
namespace python
{

void initBindingBaseNode(pybind11::module& m);

// This trampoline binding class is actually never used, because 
// addChild and removeChild virtual methods cannot be bound
// since casting to the base type sofa::sptr< BaseNode > fails here. 
// The issue is related to multiple inheritance and having 
// a custom holder type ( here sofa::sptr )
// More info here: https://stackoverflow.com/questions/49178231/pybind11-multiple-inheritance-with-custom-holder-type-fails-to-cast-to-base-type
// UPDATE: the underlying issue is now fixed in a custom pybind11 patch: [pybind11]  FIX: enable implicit casts for boost::intrusive_ptr-like custom holders and bindings with multiple_inheritance enabled (Ref T10602)
// however this trampoline is not really useful in practice so still not in use.

template < typename TSofaNode >
class PySofaBaseNode : public PySofaBase< TSofaNode >
{
public:
    using BaseNode = sofa::core::objectmodel::BaseNode;
    using BaseObject = sofa::core::objectmodel::BaseObject;

    void addChild(sofa::sptr< BaseNode > node) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaNode, addChild, node);
    }

    void removeChild(sofa::sptr< BaseNode > node) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaNode, removeChild, node);
    }

    bool addObject(sofa::sptr< BaseObject > obj) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaNode, addObject, obj);
    }

    bool removeObject(sofa::sptr< BaseObject > obj) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaNode, removeObject, obj);
    }

    using PySofaBase< TSofaNode >::PySofaBase;
};

}

}

#endif // ISSOFA_PYTHON_BASENODEBINDING_H


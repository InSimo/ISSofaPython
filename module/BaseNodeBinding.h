/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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


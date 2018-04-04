/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_NODEBINDING_H
#define ISSOFA_PYTHON_NODEBINDING_H

#include <pybind11/pybind11.h>
//#include "BaseNodeBinding.h"
#include "BaseContextBinding.h"
#include <sofa/simulation/common/Node.h>

namespace sofa
{
namespace python
{

void initBindingNode(pybind11::module& m);

template < typename TSofaNode >
class PySofaNode : public PySofaBaseContextObject< TSofaNode >
{
public:
    sofa::sptr< sofa::simulation::Node > createChild(const std::string& name) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::sptr< sofa::simulation::Node >, TSofaNode, createChild, name);
    }

    bool addObject(sofa::sptr<sofa::core::objectmodel::BaseObject> obj) override
    {
        PYBIND11_OVERLOAD(bool, TSofaNode, addObject, obj);
    }

    bool removeObject(sofa::sptr<sofa::core::objectmodel::BaseObject> obj) override
    {
        PYBIND11_OVERLOAD(bool, TSofaNode, removeObject, obj);
    }

    using PySofaBaseContextObject< TSofaNode >::PySofaBaseContextObject;

};

}

}

#endif // ISSOFA_PYTHON_GNODEBINDING_H

/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_GNODEBINDING_H
#define ISSOFA_PYTHON_GNODEBINDING_H

#include <pybind11/pybind11.h>
#include "NodeBinding.h"
#include <sofa/simulation/tree/GNode.h>
namespace sofa
{
namespace python
{

void initBindingGNode(pybind11::module& m);


class PySofaGNode : public PySofaNode< sofa::simulation::tree::GNode >
{
public:

    typedef sofa::simulation::tree::GNode GNode;
    typedef sofa::core::objectmodel::BaseNode BaseNode;

    using PySofaNode< sofa::simulation::tree::GNode >::PySofaNode;

    PySofaGNode() :
    PySofaGNode("", nullptr)
    {

    }

    PySofaGNode(const std::string& name):
    PySofaGNode(name,nullptr)
    {

    }

    sofa::sptr< sofa::simulation::Node > createChild(const std::string& name) override
    {
        PYBIND11_OVERLOAD(sofa::sptr< sofa::simulation::Node >, GNode, createChild, name);
    }


};

}

}

#endif 

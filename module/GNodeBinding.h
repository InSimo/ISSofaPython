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

    //sofa::sptr< sofa::simulation::Node > createChild(const std::string& name) override
    //{
    //    PYBIND11_OVERLOAD(sofa::sptr< sofa::simulation::Node >, GNode, createChild, name);
    //}


};

}

}

#endif 

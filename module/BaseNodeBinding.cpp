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

#include "BaseNodeBinding.h"
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/tree/TreeSimulation.h>

namespace sofa
{

namespace python
{

using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::Base;

std::string getPath(const BaseNode* node)
{
    return "@"+node->getPathName();
}

namespace internal
{

pybind11::object getNode(std::string path)
{
    sofa::sptr<sofa::simulation::Node> root = sofa::simulation::tree::getSimulation()->GetRoot();
    sofa::simulation::Node* node;
    root->findLinkDest(node, path, nullptr);
    // sofa::sptr<GNode> gNodeRoot = sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
    if(node)
    {
        return pybind11::cast(node);
    }
    else
    {
        return pybind11::none();
    }
}

}


void initBindingBaseNode(pybind11::module& m)
{
    pybind11::class_< BaseNode, Base, 
                      PySofaBaseNode< BaseNode >, 
                      sofa::sptr< BaseNode > > baseNode(m, "BaseNode", pybind11::multiple_inheritance());

    baseNode
        .def("addObject", &BaseNode::addObject)
        .def("removeObject", &BaseNode::removeObject)
        .def("addChild", &BaseNode::addChild)
        .def("removeChild", &BaseNode::removeChild)
        .def("getPath", &getPath) // deprecated, prefer the less verbose "path" property
        .def_property_readonly("path", &getPath)
        .def("getContext", pybind11::overload_cast<>(&BaseNode::getContext, pybind11::const_), pybind11::return_value_policy::reference)
        ;
    
    m.def("getNode", &internal::getNode);
}

}

}
  
  
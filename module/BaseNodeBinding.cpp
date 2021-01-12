/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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
  
  
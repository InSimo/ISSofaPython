/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/
#include "GNodeBinding.h"
#include "ObjectFactoryBinding.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/tree/GNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#include <pybind11/iostream.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseNode;
using sofa::simulation::Node;
using sofa::simulation::tree::GNode;
using sofa::core::objectmodel::New;
using sofa::core::objectmodel::BaseObject;

namespace internal
{
pybind11::object createObject(Node* node, pybind11::args args, pybind11::kwargs kwargs)
{
    return sofa::python::createObject(sofa::core::ObjectFactory::getInstance(), node, args, kwargs);
}

pybind11::object createRootNode(const std::string& name)
{
    sofa::sptr<Node> root = sofa::simulation::tree::getSimulation()->createNewGraph(name);
    sofa::sptr<GNode> gNodeRoot = sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
    return bindDataAndLinks(gNodeRoot);
}

pybind11::object createNode(const std::string& name)
{
    sofa::sptr<Node> node = sofa::simulation::tree::getSimulation()->createNewNode(name);
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    return bindDataAndLinks(gNode);
}

pybind11::object getRoot()
{
    sofa::sptr<Node> root = sofa::simulation::tree::getSimulation()->GetRoot();
    sofa::sptr<GNode> gNodeRoot = sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
    return bindDataAndLinks(gNodeRoot);
}

pybind11::object getChildren(sofa::sptr<Node> node)
{
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    sofa::simulation::Node::Children children = gNode->getChildren();
    pybind11::list pyList;
    for (auto& child : children)
    {
        pyList.append(child);
    }
    return pyList;
}

pybind11::object createChild(sofa::sptr<Node> node, const std::string& name)
{
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    sofa::sptr<Node> nChild = gNode->createChild(name);
    sofa::sptr<GNode> gChild = sofa::core::objectmodel::SPtr_static_cast<GNode>(nChild);
    return bindDataAndLinks(gChild);
}

}

void initBindingGNode(pybind11::module& m)
{
    pybind11::class_<GNode, Node,
        PySofaGNode,
        sofa::sptr< GNode > > gNode(m, "GNode", pybind11::dynamic_attr());
           
    gNode.def(pybind11::init<>())
         .def("createObject", &internal::createObject)
         .def("addChild", [](GNode* instance, GNode::SPtr child) { instance->addChild(child); } )
         .def("removeChild", [](GNode* instance, GNode::SPtr child) { instance->removeChild(child); } )
         .def("createChild", &internal::createChild)
         .def("getChildren", &internal::getChildren)
         ;

    m.def("createRootNode", &internal::createRootNode);
    m.def("createNode", &internal::createNode);
    m.def("getRoot", &internal::getRoot);
}

}

}
    
    


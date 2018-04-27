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

sofa::sptr<GNode> createRootNode(const std::string& name)
{
    sofa::sptr<Node> root = sofa::simulation::tree::getSimulation()->createNewGraph(name);
    return sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
}

sofa::sptr<GNode> createNode(const std::string& name)
{
    sofa::sptr<Node> node = sofa::simulation::tree::getSimulation()->createNewNode(name);
    return sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
}

}

void initBindingGNode(pybind11::module& m)
{
    pybind11::class_<GNode, Node,
                     PySofaGNode, 
                     sofa::sptr< GNode > > gNode(m, "GNode");
    gNode.def("createObject", &internal::createObject)
         .def("addChild", [](GNode* instance, GNode::SPtr child) { instance->addChild(child); } )
         .def("removeChild", [](GNode* instance, GNode::SPtr child) { instance->removeChild(child); } )
         ;

    m.def("createRootNode", &internal::createRootNode );
    m.def("createNode", &internal::createNode );
}

}

}
    
    


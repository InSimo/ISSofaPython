/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/
#include "GNodeBinding.h"
#include "ObjectFactoryBinding.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/behavior/BaseAnimationLoop.h>
#include <sofa/simulation/common/ScriptEvent.h>
#include <sofa/simulation/tree/GNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#include <pybind11/iostream.h>

#include <sofa/core/ExecParams.h>
#include "PythonVisitor.h"

#include <pybind11/pybind11.h>
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
    return pybind11::cast(gNodeRoot);
}

pybind11::object createNode(const std::string& name)
{
    sofa::sptr<Node> node = sofa::simulation::tree::getSimulation()->createNewNode(name);
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);

    return pybind11::cast(gNode);
}

pybind11::object getRoot()
{
    sofa::sptr<Node> root = sofa::simulation::tree::getSimulation()->GetRoot();
    sofa::sptr<GNode> gNodeRoot = sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
    return pybind11::cast(gNodeRoot);
}

pybind11::object getRootFromNode(Node* /* node */)
{
    return getRoot();
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

pybind11::object getChild(sofa::sptr<Node> node, const std::string& name)
{
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    auto child = gNode->getChild(name);
    return pybind11::cast(child);
}

pybind11::object createChild(sofa::sptr<Node> node, const std::string& name)
{
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    sofa::sptr<Node> nChild = gNode->createChild(name);
    sofa::sptr<GNode> gChild = sofa::core::objectmodel::SPtr_static_cast<GNode>(nChild);

    pybind11::object pyObj = pybind11::cast(gChild);

#ifdef ISSOFAPYTHON_CREATE_CHILD_HOOK
    /* See the associated CMake variable.
        It allows to reproduce the behavior of SofaPython that automatically initializes nodes
        created from python scripts controllers. But here it is done in a more generic way, and
        the project can register a custom hook function to do something specific with created nodes.
        (can be used on legacy projects being migrated from SofaPython to ISSofaPython,
        but the the interest is otherwise limited...) */ 
    pybind11::dict pyHooks = pybind11::module::import("sofa").attr("HOOKS");
    pybind11::object createChildHook = pyHooks.attr("get")("createChild");
    if (!createChildHook.is_none()) {
        createChildHook(pyObj);
    }
#endif

    return pyObj;
}

pybind11::object sendScriptEvent(sofa::sptr<Node> node, const std::string& eventName, pybind11::object& data)
{
    // TODO: what if data is not None or an empty string?
    //       For now it seems we don't need a PythonScriptEvent that can handle data, like the one existing in SofaPython
    if (!(data.is_none() || pybind11::str(data).cast<std::string>().empty()))
    {
        std::cerr << "<ISSofaPython> sendScriptEvent WARNING: data is not None or is not an empty string, value not handled: " << pybind11::str(data).cast<std::string>() << std::endl;
    }

    sofa::core::behavior::BaseAnimationLoop* m_animationloop;
    node->getRootContext()->get(m_animationloop);
    m_animationloop->setSimulationToSleep(false);  // See T10110

    sofa::simulation::ScriptEvent event(node, eventName.c_str());
    sofa::sptr<GNode> gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);
    gNode->propagateEvent(sofa::core::ExecParams::defaultInstance(), &event);
    return pybind11::none();
}


pybind11::object executeVisitor(sofa::sptr<Node> node, pybind11::object& pyVisitor)
{
    sofa::simulation::PythonVisitor visitor(sofa::core::ExecParams::defaultInstance(), pyVisitor);
    node->executeVisitor(&visitor);
    return pybind11::none();
}

}

void initBindingGNode(pybind11::module& m)
{
    pybind11::class_<GNode, Node,
        PySofaGNode,
        sofa::sptr< GNode > > gNode(m, "GNode");

    gNode.def(pybind11::init<>())
         .def("createObject", &internal::createObject)
         .def("removeObject", &GNode::removeObject)
         .def("addChild", [](GNode* instance, GNode::SPtr child) { instance->addChild(child); } )
         .def("removeChild", [](GNode* instance, GNode::SPtr child) { instance->removeChild(child); } )
         .def("createChild", &internal::createChild)
         .def("getChildren", &internal::getChildren)
         .def("getChild", &internal::getChild)
         .def("getPathName", &Node::getPathName)
         .def("detachFromGraph", &Node::detachFromGraph)
         .def("init", [](GNode* instance) { instance->init(sofa::core::ExecParams::defaultInstance()); } )
         .def("sendScriptEvent", &internal::sendScriptEvent)
         .def("executeVisitor", &internal::executeVisitor)
         .def("getRoot", &internal::getRootFromNode)

         // A property setter does not work here, because the Base binding has an implementation of __setattr__ that just sets the attribute on the py object
         .def_property_readonly("active", &sofa::core::objectmodel::Context::isActive)
         .def("setActive", &sofa::core::objectmodel::Context::setActive) // Added because the property setter was not working...

         .def("isInitialized", &Node::isInitialized)
         ;

    m.def("createRootNode", &internal::createRootNode);
    m.def("createNode", &internal::createNode);
    m.def("getRoot", &internal::getRoot);
}

}

}
    
    


/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/
#include "GNodeBinding.h"
#include "ObjectFactoryBinding.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/tree/GNode.h>

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

}

void initBindingGNode(pybind11::module& m)
{
    pybind11::class_<GNode, Node,
                     PySofaGNode, 
                     sofa::sptr< GNode > > gNode(m, "GNode");
    gNode.def(pybind11::init<>())
         .def(pybind11::init<const std::string&>())
         .def("createObject",&internal::createObject, 
              pybind11::call_guard<pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect>())
         .def("addChild", [](GNode* instance, GNode::SPtr child) { instance->addChild(child); } )
         .def("removeChild", [](GNode* instance, GNode::SPtr child) { instance->removeChild(child); } )
         ;

    m.def("CreateObject", &internal::createObject, 
          pybind11::call_guard<pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect>() );
}

}

}
    
    


/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "NodeBinding.h"
#include "BaseNodeBinding.h"
#include "ContextBinding.h"
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/Context.h>

namespace sofa
{
namespace python
{

using sofa::simulation::Node;
using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::Context;
using sofa::core::objectmodel::BaseContext;


void initBindingNode(pybind11::module& m)
{

    pybind11::class_<Node, BaseNode, Context,  
                     PySofaNode< Node >,
                     sofa::sptr< Node > > node(m, "Node", pybind11::multiple_inheritance() ); 

    //node.def("createChild", &Node::createChild );
}

}

}
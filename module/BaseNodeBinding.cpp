/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseNodeBinding.h"

namespace sofa
{

namespace python
{

using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::Base;

void initBindingBaseNode(pybind11::module& m)
{
    pybind11::class_< BaseNode, Base, 
                      PySofaBaseNode< BaseNode >, 
                      sofa::sptr< BaseNode > > baseNode(m, "BaseNode");

    baseNode
        .def("addObject", &BaseNode::addObject)
        .def("removeObject", &BaseNode::removeObject)
        .def("addChild", &BaseNode::addChild)
        .def("removeChild", &BaseNode::removeChild)
        ;
}

}

}
  
  
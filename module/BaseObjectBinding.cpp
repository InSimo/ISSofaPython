#include "BaseObjectBinding.h"
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/BaseNode.h>
namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::New;
using sofa::core::objectmodel::BaseNode;

std::string getPath(const BaseObject* obj)
{
    std::string path;
    if (const BaseNode* node = BaseNode::DynamicCast(obj->getContext()))
    {
        path += "@"+node->getPathName() + "/" + obj->getName();;
    }
    return path;
}

void initBindingBaseObject(pybind11::module& m)
{
    pybind11::class_<BaseObject, Base, 
                     PySofaBaseObject<BaseObject>, 
                     sofa::sptr<BaseObject> >(m, "BaseObject", pybind11::dynamic_attr())
        .def(pybind11::init<>() )
        .def("init", &BaseObject::init)
        .def("bwdInit", &BaseObject::bwdInit)
        .def("reinit", &BaseObject::reinit)
        .def("handleEvent", &BaseObject::handleEvent)
        .def("cleanup",&BaseObject::cleanup)
        .def("reset",&BaseObject::reset)
        .def("getMutableContext", pybind11::overload_cast<>( &BaseObject::getContext), pybind11::return_value_policy::reference)
        .def("getContext", pybind11::overload_cast<>(&BaseObject::getContext, pybind11::const_), pybind11::return_value_policy::reference)
        .def("getPath", &getPath)
        ;
}

}

}

#include "BaseObjectBinding.h"
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>


namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::New;
using sofa::core::objectmodel::BaseNode;

namespace internal
{

std::string getPath(const BaseObject* obj)
{
    std::string path;
    if (const BaseNode* node = BaseNode::DynamicCast(obj->getContext()))
    {
        path += "@"+node->getPathName() + "/" + obj->getName();;
    }
    return path;
}

pybind11::object getObject(std::string path)
{
    sofa::sptr<sofa::simulation::Node> root = sofa::simulation::tree::getSimulation()->GetRoot();
    BaseObject* obj;
    root->findLinkDest(obj, path, nullptr);
    // sofa::sptr<GNode> gNodeRoot = sofa::core::objectmodel::SPtr_static_cast<GNode>(root);
    if(obj)
    {
        return bindDataAndLinks(obj);
    }
    else
    {
        return pybind11::none();
    }
}

pybind11::object getSlaves(sofa::sptr<BaseObject> obj)
{
    auto slaves = obj->getSlaves();
    pybind11::list pyList;
    for (auto& slave : slaves)
    {
        pyList.append(slave);
    }
    return pyList;
}

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
        .def("getPath", &internal::getPath)
        .def("getSlaves", &internal::getSlaves)
        ;

    m.def("getObject", &internal::getObject);
}

}

}

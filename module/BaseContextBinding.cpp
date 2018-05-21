#include "BaseContextBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <pybind11/stl.h>

namespace sofa
{

namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseClass;

std::vector< BaseObject::SPtr > getObjects(BaseContext* ctx, const BaseClass* classInfo, BaseContext::SearchDirection dir)
{
    std::vector< BaseObject::SPtr > all_objects;
    std::vector< BaseObject::SPtr > objects;

    ctx->get< BaseObject >(&all_objects, dir);

    for (const auto& obj : all_objects)
    {
        if ( classInfo->dynamicCast(obj.get()) )
        {
            objects.push_back(obj);
        }
    }

    return objects;
}

BaseObject::SPtr getObject(BaseContext* ctx, std::string path)
{
    BaseObject::SPtr obj;

    ctx->get<BaseObject>(obj, path);

    return obj;
}

void initBindingBaseContext(pybind11::module& m)
{

    pybind11::class_<BaseContext, 
        Base, PySofaBaseContextObject<BaseContext>, 
        sofa::sptr<BaseContext> > baseContext(m, "BaseContext");
    
    pybind11::enum_<BaseContext::SearchDirection>(baseContext, "SearchDirection")
        .value("SearchUp", BaseContext::SearchDirection::SearchUp)
        .value("Local", BaseContext::SearchDirection::Local)
        .value("SearchDown", BaseContext::SearchDirection::SearchDown)
        .value("SearchRoot", BaseContext::SearchDirection::SearchRoot)
        .value("SearchParents", BaseContext::SearchDirection::SearchParents)
        .export_values();

    baseContext.def("addObject", &BaseContext::addObject )
               .def("removeObject", &BaseContext::removeObject)
               .def("getObject", &getObject)
               .def("getObjects", &getObjects, pybind11::arg("classInfo"), pybind11::arg("dir") = BaseContext::SearchUp )
               .def("getDt",&BaseContext::getDt )
               .def("getTime",&BaseContext::getTime )
        ;


}

}

}
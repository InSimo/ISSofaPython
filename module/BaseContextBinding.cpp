#include "BaseContextBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/TemplatesAliases.h>
#include <pybind11/stl.h>

namespace sofa
{

namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseClass;
using sofa::core::ObjectFactory;

std::vector< BaseObject::SPtr > getAllObjects(BaseContext* ctx, BaseContext::SearchDirection dir)
{
    std::vector< BaseObject::SPtr > allObjects;
    ctx->get< BaseObject >(&allObjects, dir);
    return  allObjects;
}

void appendMatchingObjects( std::vector< pybind11::object >& objects, 
                            const std::vector< BaseObject::SPtr>& allObjects, 
                            const BaseClass* classInfo)
{
    for (const auto& obj : allObjects)
    {
        if ( classInfo->dynamicCast(obj.get()) )
        {
            objects.push_back(bindDataAndLinks(obj));
        }
    }
}

std::vector< const BaseClass* > getObjectBaseClass(std::string className, std::string templateName)
{
    std::vector< const BaseClass* > objectClass;

    const auto& registry = ObjectFactory::getInstance()->getRegistry();
    templateName = sofa::defaulttype::TemplateAliases::resolveAlias(templateName);
    auto itEntry = registry.find(className);
    if (itEntry != registry.end())
    {
        ObjectFactory::ClassEntry::SPtr entry = itEntry->second;
        if (!templateName.empty())
        {
            auto itCreator = entry->creatorMap.find(templateName);
            if (itCreator != entry->creatorMap.end())
            {
                objectClass.push_back(itCreator->second->getClass());
            }
        }
        else
        {
            for (auto kv : entry->creatorMap)
            {
                objectClass.push_back(kv.second->getClass());
            }
        }
    }

    return objectClass;

}

std::vector< pybind11::object > searchObjects(BaseContext* ctx, BaseContext::SearchDirection dir, std::string className, std::string templateName)
{
    std::vector< BaseObject::SPtr > allObjects = getAllObjects(ctx, dir);
    std::vector< pybind11::object > allBindedObjects;
    if (className.empty())
    {
        for (const auto& obj : allObjects)
        {
            allBindedObjects.push_back(bindDataAndLinks(obj));
        }
        
        return allBindedObjects;
    }
    else
    {
        std::vector< pybind11::object > objectsFound;
        std::vector< const BaseClass* > objectBaseClass = getObjectBaseClass(className, templateName);
        for (const BaseClass* baseClass : objectBaseClass)
        {
            appendMatchingObjects(objectsFound, allObjects, baseClass);
        }

        return objectsFound;
    }
}

pybind11::object searchFirstObject(BaseContext* ctx, BaseContext::SearchDirection dir, std::string className, std::string templateName)
{
    std::vector< pybind11::object > objects = searchObjects(ctx, dir, className, templateName);
    if (objects.empty())
    {
        return pybind11::object();
    }
    else
    {
        return objects[0];
    }
}

std::vector< pybind11::object > searchAllUp(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchObjects(ctx, BaseContext::SearchUp, className, templateName);
}

pybind11::object searchUp(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchFirstObject(ctx, BaseContext::SearchUp, className, templateName);
}

std::vector< pybind11::object > searchAllLocal(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchObjects(ctx, BaseContext::Local, className, templateName);
}

pybind11::object searchLocal(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchFirstObject(ctx, BaseContext::Local, className, templateName);
}

std::vector< pybind11::object > searchAllParents(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchObjects(ctx, BaseContext::SearchParents, className, templateName);
}

pybind11::object searchParents(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchFirstObject(ctx, BaseContext::SearchParents, className, templateName);
}

std::vector< pybind11::object > searchAllDown(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchObjects(ctx, BaseContext::SearchDown, className, templateName);
}

pybind11::object searchDown(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchFirstObject(ctx, BaseContext::SearchDown, className, templateName);
}

std::vector< pybind11::object > searchAllRoot(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchObjects(ctx, BaseContext::SearchRoot, className, templateName);
}

pybind11::object searchRoot(BaseContext* ctx, std::string className, std::string templateName)
{
    return searchFirstObject(ctx, BaseContext::SearchRoot, className, templateName);
}

pybind11::object getObject(BaseContext* ctx, std::string path)
{
    BaseObject::SPtr obj;
    const char atStr = '@';
    const bool found = path.empty() ? false : path[0] == atStr;
    if (found)
    {
        ctx->get<BaseObject>(obj, path.substr(1));
    }
    else
    {
        ctx->get<BaseObject>(obj, path);
    }

    if (obj)
    {
        return bindDataAndLinks(obj);
    }
    else
    {
        return pybind11::none();
    }
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

    baseContext.def("addObject", &BaseContext::addObject)
        .def("removeObject", &BaseContext::removeObject)
        .def("getObject", &getObject)
        .def("searchUp", &searchUp, pybind11::arg("className") = std::string(),
                                    pybind11::arg("templateName") = std::string())
        .def("searchAllUp", &searchAllUp, pybind11::arg("className") = std::string(),
                                                  pybind11::arg("templateName") = std::string())
        .def("searchLocal", &searchLocal, pybind11::arg("className") = std::string(),
                                          pybind11::arg("templateName") = std::string())
        .def("searchAllLocal",&searchAllLocal, pybind11::arg("className") = std::string(),
                                               pybind11::arg("templateName") = std::string())
        .def("searchParents", &searchParents, pybind11::arg("className") = std::string(),
                                              pybind11::arg("templateName") = std::string())
        .def("searchAllParents", &searchAllParents, pybind11::arg("className") = std::string(),
                                                    pybind11::arg("templateName") = std::string())
        .def("searchDown", &searchDown, pybind11::arg("className") = std::string(),
                                        pybind11::arg("templateName") = std::string())
        .def("searchAllDown", &searchAllDown, pybind11::arg("className") = std::string(),
                                              pybind11::arg("templateName") = std::string())
        .def("searchRoot", &searchRoot, pybind11::arg("className") = std::string(),
                                        pybind11::arg("templateName") = std::string())
        .def("searchAllRoot", &searchAllRoot, pybind11::arg("className") = std::string(),
                                              pybind11::arg("templateName") = std::string())
        .def("getDt",&BaseContext::getDt )
        .def("getTime",&BaseContext::getTime )
        ;
}

}

}
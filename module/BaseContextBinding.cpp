#include "BaseContextBinding.h"
#include "common.h"

#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/TemplatesAliases.h>

#include <sofa/simulation/common/Node.h>

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
            objects.push_back(getDerivedPyObject(obj.get()));
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
            allBindedObjects.push_back(getDerivedPyObject(obj.get()));
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
        return getDerivedPyObject(obj.get());
    }
    else
    {
        return pybind11::none();
    }
}

#ifdef ISSOFAPYTHON_USE_LEGACY_API

// The search* methods don't always give the same output as the former SofaPython's getObjects method:
// For eg. contextObj.searchDown('MechanicalObject') does not produce the same output as SofaPython's `getObjects('SearchDown', 'MechanicalObject')[0]`.
// Ideally we could certainly modify the search* methods, or modify python code using it,
// when migrating from SofaPython to ISSofaPython, but for now the SofaPython's implementation is added to limit regressions.
pybind11::object getObjectsLegacy(BaseContext* ctx, std::string searchDirection, std::string typeName, std::string name)
{
    ObjectFactory::ClassEntry* class_entry = typeName.empty() ? nullptr : &ObjectFactory::getInstance()->getEntry(typeName);
    sofa::core::objectmodel::BaseContext::SearchDirection searchDirectionEnum = sofa::core::objectmodel::BaseContext::Local;
    if ( !searchDirection.empty() ) 
    {
        if ( searchDirection == "SearchUp" )
        {
            searchDirectionEnum= sofa::core::objectmodel::BaseContext::SearchUp;
        }
        else if ( searchDirection == "Local" )
        {
            searchDirectionEnum= sofa::core::objectmodel::BaseContext::Local;
        }
        else if ( searchDirection == "SearchDown" )
        {
            searchDirectionEnum= sofa::core::objectmodel::BaseContext::SearchDown;
        }
        else if ( searchDirection == "SearchRoot" )
        {
            searchDirectionEnum= sofa::core::objectmodel::BaseContext::SearchRoot;
        }
        else if ( searchDirection == "SearchParents" )
        {
            searchDirectionEnum= sofa::core::objectmodel::BaseContext::SearchParents;
        }
        else 
        {
            throw std::invalid_argument("getObjects: invalid search direction, expected: 'SearchUp', 'Local', 'SearchDown', 'SearchRoot', or 'SearchParents'." );
        }
    }

    sofa::helper::vector<sofa::sptr<BaseObject>> list;
    ctx->get<BaseObject>(&list, searchDirectionEnum);
    pybind11::list pyList;
    for (unsigned int i = 0; i < list.size(); i++)
    {
        BaseObject* o = list[i].get();
        if (!class_entry || o->getClassName() == class_entry->className || class_entry->creatorMap.find(o->getClassName()) != class_entry->creatorMap.end())
        {
            if (name.empty() || name == o->getName())
            {
                pyList.append(getDerivedPyObject(o));
            }
        }
    }
    return pyList;
}

pybind11::object getParentsLegacy(BaseContext* ctx)
{
    simulation::Node* node = simulation::Node::DynamicCast(ctx);
    const core::objectmodel::BaseNode::Children& parents = node->getParents();
    pybind11::list pyList;
    for (core::objectmodel::BaseNode* parent: parents)
    {
        pyList.append(getDerivedPyObject(parent));
    }
    return pyList;
}

#endif // ISSOFAPYTHON_USE_LEGACY_API

void initBindingBaseContext(pybind11::module& m)
{

    pybind11::class_<BaseContext, 
        Base, PySofaBaseContextObject<BaseContext>, 
        sofa::sptr<BaseContext> > baseContext(m, "BaseContext", pybind11::multiple_inheritance());
    
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
        // alias of searchAllLocal
        .def("getObjects",&searchAllLocal, pybind11::arg("className") = std::string(),
                                               pybind11::arg("templateName") = std::string())
        .def("getDt",&BaseContext::getDt )
        .def("getTime",&BaseContext::getTime )

#ifdef ISSOFAPYTHON_USE_LEGACY_API
        // Legacy implementation of `getObjects` from SofaPython
        .def("getObjectsLegacy", &getObjectsLegacy, pybind11::arg("searchDirection") = std::string(), pybind11::arg("typeName") = std::string(), pybind11::arg("name") = std::string())
        // Legacy implementation of `getParents` from SofaPython
        .def("getParentsLegacy", &getParentsLegacy)
#endif
        ;
}

}

}

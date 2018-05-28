/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "ObjectFactoryBinding.h"
#include "BaseObjectBinding.h"
#include <sofa/core/ObjectFactory.h>
#include "BaseObjectDescriptionBinding.h"

#include <pybind11/stl.h>
#include <pybind11/iostream.h>


// Required to return a vector of BaseClass* whose ownership must not be transferred to python
// See https ://github.com/pybind/pybind11/issues/637

using ListCasterBase = pybind11::detail::list_caster<std::vector<const sofa::core::objectmodel::BaseClass *>, const  sofa::core::objectmodel::BaseClass*>;

namespace pybind11 
{
namespace detail 
{

template<> struct type_caster<std::vector<const sofa::core::objectmodel::BaseClass *>> : ListCasterBase {
    static handle cast(const std::vector<const sofa::core::objectmodel::BaseClass *> &src, return_value_policy, handle parent) {
        return ListCasterBase::cast(src, return_value_policy::reference, parent);
    }
    static handle cast(const std::vector<const sofa::core::objectmodel::BaseClass *> *src, return_value_policy pol, handle parent) {
        return cast(*src, pol, parent);
    }
};

}

}

namespace sofa
{
namespace python
{

using sofa::core::ObjectFactory;
using sofa::core::objectmodel::BaseClass;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseObjectDescription;

std::vector< const BaseClass* > getClass(ObjectFactory* factory, std::string className)
{
    std::vector< const BaseClass* > result;
    if (factory->hasCreator(className))
    {
        const auto& classEntry = factory->getEntry(className);
        for (auto kv : classEntry.creatorMap)
        {
            result.push_back(kv.second->getClass());
        }
    }

    return result;
}



pybind11::object createObject(ObjectFactory* factory, BaseContext* ctx, pybind11::args args, pybind11::kwargs kwargs)
{
    BaseObjectDescription desc = createBaseObjectDescription(args, kwargs);
    sofa::sptr<BaseObject > obj = factory->createObject(ctx, &desc);

    if (obj == nullptr)
    {
        throw std::invalid_argument("createObject failed with: " + desc.getName());
    }

    // store the source location of this object
    //PyFrameObject* frame = PyEval_GetFrame();
    PyThreadState *tstate = PyThreadState_GET();
    if (NULL != tstate && NULL != tstate->frame)
    {
        PyFrameObject *frame = tstate->frame;
        // int line = frame->f_lineno;
        /*
        frame->f_lineno will not always return the correct line number
        you need to call PyCode_Addr2Line().
        */
        int line = PyCode_Addr2Line(frame->f_code, frame->f_lasti);
        const char *filename = PyString_AsString(frame->f_code->co_filename);
        //const char *funcname = PyString_AsString(frame->f_code->co_name);
        //printf("    %s(%d): %s\n", filename, line, funcname);
        //frame = frame->f_back;
        obj->setSourceFile(filename, line, 0);
    }


    return bindDataAndLinks(obj);
}


void initBindingObjectFactory(pybind11::module& m)
{
    pybind11::class_<ObjectFactory, 
                    std::unique_ptr<ObjectFactory, pybind11::nodelete> // singleton class that must not be deleted by python 
                    >(m, "ObjectFactory")
        .def(pybind11::init([](void) { return ObjectFactory::getInstance(); }), pybind11::return_value_policy::reference)
        .def_static("getInstance", &ObjectFactory::getInstance, pybind11::return_value_policy::reference)
        .def("createObject", &ObjectFactory::createObject)
        .def("getClass", &getClass )
        ;
}


}


}


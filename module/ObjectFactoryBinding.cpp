/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "ObjectFactoryBinding.h"
#include "BaseObjectBinding.h"
#include "BaseDataBinding.h"
#include "BaseObjectDescriptionBinding.h"
#include "common.h"
#include "Exceptions.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

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
using sofa::core::objectmodel::BaseData;

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
    BaseObjectDescription objDescription = createBaseObjectDescription(args,kwargs);

    sofa::sptr<BaseObject > obj = factory->createObject(ctx, &objDescription);

    if (obj == nullptr)
    {
        throw pybind11::type_error("createObject failed with: " + std::string(pybind11::str(args))  + " " + std::string(pybind11::str(kwargs)) );
    }

    std::vector<std::string> unAccessedAttributes;
    objDescription.getAttributeList(unAccessedAttributes, false);

    if (!unAccessedAttributes.empty())
    {
        std::ostringstream oss;
        std::ostream_iterator<std::string::value_type> outIt(oss, ", ");

        for (std::size_t i=0;i<unAccessedAttributes.size();++i)
        {
            oss << unAccessedAttributes[i];
            const bool isLast = i == unAccessedAttributes.size() - 1;
            if (!isLast)
            {
                oss << ", ";
            }
        }
        throw SofaAttributeError(obj.get(), "unused attribute(s) : \"" + oss.str() + "\"");
    }

    pybind11::dict dict(kwargs);

    // process elements in the dict that are not taken care of by BaseObjectDescription
    for (auto item : dict)
    {
        std::string attr = std::string(pybind11::str(item.first));     

        if (!isPythonTypeHandledByBaseObjectDescription(item.second))
        {
            pybind11::object value = pybind11::reinterpret_borrow<pybind11::object>(item.second);

            // We allow passing python none objects only in the createObject method, in which case assignemt is simply ignored.
            if (value.is_none()) continue;

            // it can only be a data, since links are initialized from strings, which are taken care of by BaseObjectDescription.
            BaseData* data   = obj->findData(attr); 
            if (data)
            {
                // will throw invalid_argument if the python object cannot be converted to something compatible with the data type
                setDataValueFromPyObject(data, value);
            }
            else throw SofaAttributeError(obj.get(), "unused attribute: \"" + attr + "\"");
        }
    }

    // store the source location of this object
    //PyFrameObject* frame = PyEval_GetFrame();
    PyThreadState *tstate = PyThreadState_GET();
    if (NULL != tstate && NULL != tstate->frame)
    {
        PyFrameObject *frame = tstate->frame;
        int depth = 6; // max number of frames to capture
        do
        {
            // int line = frame->f_lineno;
            /*
              frame->f_lineno will not always return the correct line number
              you need to call PyCode_Addr2Line().
            */
             int line = PyCode_Addr2Line(frame->f_code, frame->f_lasti);
             const char *filename = nullptr;
             // https://stackoverflow.com/questions/22487780/what-do-i-use-instead-of-pystring-asstring-when-loading-a-python-module-in-3-3
             if (PyUnicode_Check(frame->f_code->co_filename)) {
                 PyObject* tempBytes = PyUnicode_AsEncodedString(frame->f_code->co_filename, "UTF-8", "strict");
                 if (tempBytes)
                 {
                     filename = PYBIND11_BYTES_AS_STRING(tempBytes);
                     obj->addSourceFile(filename, line, 0);
                     Py_DECREF(tempBytes);
                 }
             } else if (PyBytes_Check(frame->f_code->co_filename)) {
                 filename = PYBIND11_BYTES_AS_STRING(frame->f_code->co_filename);
                 obj->addSourceFile(filename, line, 0);
             }
             //const char *funcname = PyString_AsString(frame->f_code->co_name);
             //printf("    %s(%d): %s\n", filename, line, funcname);
             frame = frame->f_back;
        }
        while (NULL != frame && --depth > 0);
    }


    return getDerivedPyObject(obj.get());
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


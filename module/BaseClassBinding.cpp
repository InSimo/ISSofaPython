/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseClassBinding.h"
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::BaseClass;

void initBindingBaseClassInfo(pybind11::module& m)
{
    using BaseClassInfo  = sofa::core::objectmodel::BaseClassInfo;
    pybind11::class_<BaseClassInfo,
                    std::unique_ptr<BaseClassInfo, pybind11::nodelete> >(m, "BaseClassInfo")
        .def_readonly("fullTypeName", &BaseClassInfo::fullTypeName)
        .def_readonly("namespaceName", &BaseClassInfo::namespaceName)
        .def_readonly("className", &BaseClassInfo::className)
        .def_readonly("templateName", &BaseClassInfo::templateName)
        .def_readonly("shortName", &BaseClassInfo::shortName)
        .def_readonly("targetNames", &BaseClassInfo::targetNames)
        ;
}

void initBindingBaseClass(pybind11::module& m)
{
    return initBindingBaseRootClass<sofa::core::objectmodel::Base>(m, "BaseClass");
}

void initBindingEventClass(pybind11::module& m)
{
    return initBindingBaseRootClass<sofa::core::objectmodel::Event>(m, "EventClass");

}


}


}
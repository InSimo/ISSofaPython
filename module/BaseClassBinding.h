/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASECLASSBINDING_H
#define ISSOFA_PYTHON_BASECLASSBINDING_H


#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <sofa/defaulttype/BaseClass.h>

namespace sofa
{
namespace python
{

void initBindingBaseClassInfo(pybind11::module& m);

void initBindingBaseClass(pybind11::module& m);

void initBindingEventClass(pybind11::module& m);

template< typename TRootType > 
void initBindingBaseRootClass(pybind11::module& m, const std::string& typeName )
{
    using TBaseRootClass = sofa::defaulttype::BaseRootClass<TRootType>;
    using BaseClassInfo  = sofa::defaulttype::BaseClassInfo;

    pybind11::class_<TBaseRootClass, BaseClassInfo,
        std::unique_ptr<TBaseRootClass, pybind11::nodelete> >(m, typeName.c_str())
        .def("dumpInfo", [](const TBaseRootClass* rootClass) { 
                        pybind11::scoped_ostream_redirect redir; 
                        rootClass->dumpInfo(std::cout, 0); std::cout << std::flush; 
                       })
        .def("dumpHierarchy", [](const TBaseRootClass* rootClass) {
                        pybind11::scoped_ostream_redirect redir;
                        rootClass->dumpHierarchy(std::cout, 0); std::cout << std::flush;
                       })
        ;
}




}


}

#endif // ISSOFA_PYTHON_BASECLASSBINDING_H

/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*            (c) 2006-2021 MGH, INRIA, USTL, UJF, CNRS, InSimo                *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

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

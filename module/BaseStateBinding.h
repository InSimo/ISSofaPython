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

#ifndef ISSOFA_PYTHON_BASESTATEBINDING_H
#define ISSOFA_PYTHON_BASESTATEBINDING_H

#include "BaseObjectBinding.h"

#include <pybind11/pybind11.h>

namespace sofa
{

namespace python
{

void initBindingBaseState(pybind11::module& m);

template< class TSofaObject >
class PySofaBaseState : public PySofaBaseObject<TSofaObject>
{
public:

    void isClassMacroUsed(TSofaObject* sofaObject) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, isClassMacroUsed, sofaObject);
    }
    
    int getSize() const override
    {
        PYBIND11_OVERLOAD_PURE(int, TSofaObject, getSize, );
    }

    void resize(int vsize) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, resize, vsize);
    }

    sofa::core::objectmodel::BaseData* baseWrite(sofa::core::VecId v) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::core::objectmodel::BaseData*, TSofaObject, baseWrite, v);
    }

    const sofa::core::objectmodel::BaseData* baseRead(sofa::core::ConstVecId v) const override
    {
        PYBIND11_OVERLOAD_PURE(const sofa::core::objectmodel::BaseData*, TSofaObject, baseRead, v);
    }

    using PySofaBaseObject<TSofaObject>::PySofaBaseObject; // Inherit constructors
};

}


}


#endif // ISSOFA_PYTHON_BASESTATEBINDING_H

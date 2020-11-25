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

#ifndef ISSOFA_PYTHON_EVENTBINDING_H
#define ISSOFA_PYTHON_EVENTBINDING_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{
namespace python
{

void initBindingEvent(pybind11::module& m);


template <class TSofaEvent>
class PyBaseEvent : public TSofaEvent
{
public:
    typedef sofa::defaulttype::BaseRootClass<sofa::core::objectmodel::Event> EventClass;

    using TSofaEvent::TSofaEvent;

    const EventClass* getClass() const override
    {
        PYBIND11_OVERLOAD_PURE(const EventClass*, TSofaEvent, getClass, );
    }

    const char* getClassName() const override
    {
        PYBIND11_OVERLOAD(const char*, TSofaEvent, getClassName, );
    }
};

template <class TSofaEvent>
class PyEvent : public PyBaseEvent<TSofaEvent>
{
public:
    typedef sofa::defaulttype::BaseRootClass<sofa::core::objectmodel::Event> EventClass;

    using PyBaseEvent<TSofaEvent>::PyBaseEvent;

    const EventClass* getClass() const override
    {
        PYBIND11_OVERLOAD(const EventClass*, TSofaEvent, getClass, );
    }
};



}

}


#endif // !ISSOFA_PYTHON_EVENTBINDING_H

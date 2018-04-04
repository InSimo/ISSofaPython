/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_EVENTBINDING_H
#define ISSOFA_PYTHON_EVENTBINDING_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/core/objectmodel/BaseClass.h>

namespace sofa
{
namespace python
{

void initBindingEvent(pybind11::module& m);


template <class TSofaEvent>
class PyBaseEvent : public TSofaEvent
{
public:
    typedef sofa::core::objectmodel::BaseRootClass<sofa::core::objectmodel::Event> EventClass;

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
    typedef sofa::core::objectmodel::BaseRootClass<sofa::core::objectmodel::Event> EventClass;

    using PyBaseEvent<TSofaEvent>::PyBaseEvent;

    const EventClass* getClass() const override
    {
        PYBIND11_OVERLOAD(const EventClass*, TSofaEvent, getClass, );
    }
};



}

}


#endif // !ISSOFA_PYTHON_EVENTBINDING_H

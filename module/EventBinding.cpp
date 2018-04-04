/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "EventBinding.h"
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Event;


void initBindingEvent(pybind11::module& m)
{
    pybind11::class_<Event, PyBaseEvent<Event> >(m, "Event")
        .def("getClass", &Event::getClass, pybind11::return_value_policy::reference)
        .def("getClassName", &Event::getClassName, pybind11::return_value_policy::copy)
        ;

    m.def("GetEventRootClass", []() -> const sofa::core::objectmodel::BaseRootClass<Event>* { return Event::GetClass();  });
}

}

}
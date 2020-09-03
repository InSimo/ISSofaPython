/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "EventBinding.h"
#include <sofa/core/objectmodel/Event.h>
#include <sofa/simulation/common/AnimateBeginEvent.h>
#include <sofa/simulation/common/AnimateEndEvent.h>
#include <sofa/simulation/common/CollisionBeginEvent.h>
#include <sofa/simulation/common/CollisionEndEvent.h>
#include <sofa/simulation/common/IntegrateBeginEvent.h>
#include <sofa/simulation/common/IntegrateEndEvent.h>
#include <sofa/core/objectmodel/KeypressedEvent.h>
#include <sofa/simulation/common/ScriptEvent.h>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Event;
using sofa::simulation::AnimateBeginEvent;
using sofa::simulation::AnimateEndEvent;
using sofa::simulation::CollisionBeginEvent;
using sofa::simulation::CollisionEndEvent;
using sofa::simulation::IntegrateBeginEvent;
using sofa::simulation::IntegrateEndEvent;
using sofa::core::objectmodel::KeypressedEvent;
using sofa::simulation::ScriptEvent;

namespace internal
{
    sofa::simulation::Node* getSender(ScriptEvent* event)
    {
        return event->getSender().get();
    }
}

void initBindingEvent(pybind11::module& m)
{
    pybind11::class_<Event, PyBaseEvent<Event> >(m, "Event")
        .def("getClass", &Event::getClass, pybind11::return_value_policy::reference)
        .def("getClassName", &Event::getClassName, pybind11::return_value_policy::copy)
        ;

    pybind11::class_<AnimateBeginEvent, Event, PyEvent<AnimateBeginEvent> >(m, "AnimateBeginEvent")
        .def(pybind11::init<double>())
        ;
    pybind11::class_<AnimateEndEvent, Event, PyEvent<AnimateEndEvent> >(m, "AnimateEndEvent")
        .def(pybind11::init<double>())
        ;
    pybind11::class_<CollisionBeginEvent, Event, PyEvent<CollisionBeginEvent> >(m, "CollisionBeginEvent")
        .def(pybind11::init<>())
        ;
    pybind11::class_<CollisionEndEvent, Event, PyEvent<CollisionEndEvent> >(m, "CollisionEndEvent")
        .def(pybind11::init<>())
        ;
    pybind11::class_<IntegrateBeginEvent, Event, PyEvent<IntegrateBeginEvent> >(m, "IntegrateBeginEvent")
        .def(pybind11::init<>())
        ;
    pybind11::class_<IntegrateEndEvent, Event, PyEvent<IntegrateEndEvent> >(m, "IntegrateEndEvent")
        .def(pybind11::init<>())
        ;
    pybind11::class_<KeypressedEvent, Event, PyEvent<KeypressedEvent> >(m, "KeypressedEvent")
         .def(pybind11::init<char>())
         .def("getKey", &KeypressedEvent::getKey)
         ;
    pybind11::class_<ScriptEvent, Event, PyEvent<ScriptEvent> >(m, "ScriptEvent")
         .def("getSender", &internal::getSender)
         .def("getEventName", &ScriptEvent::getEventName)
         .def("getEventString", &ScriptEvent::getEventString)
         ;

    m.def("GetEventRootClass", []() -> const sofa::defaulttype::BaseRootClass<Event>* { return Event::GetClass();  });

}

}

}

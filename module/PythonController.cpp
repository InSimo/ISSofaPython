/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PythonController.h"
#include "BaseObjectBinding.h"

#include <sofa/core/ObjectFactory.h>

#include <pybind11/functional.h>

namespace sofa
{
namespace python
{


int PythonControllerClass = core::RegisterObject("Controller class to handle sofa events with python").add<PythonController>();

PythonController::PythonController()
:sofa::core::objectmodel::BaseObject()
{
    this->f_listening.setValue(true);
}

PythonController::~PythonController()
{

}

void PythonController::handleEvent(SofaEvent* e)
{
    auto it = m_callbackMap.find(std::string(e->getClassName()));

    if (it!= m_callbackMap.end())
    {
        it->second(e);
    }
}

void PythonController::cleanup()
{
    // The callback map is likely to hold function pointers that are member methods of a 
    // python class that derive from PythonController.
    // We therefore must clear it to ensure proper object destruction.
    m_callbackMap.clear();
}


void PythonController::addCallback(sofa::core::objectmodel::Event* e, const HandleEventCallback& callback)
{
    m_callbackMap[std::string(e->getClassName())] = callback;
}


void initBindingPythonController(pybind11::module& m)
{
    using sofa::core::objectmodel::BaseObject;
    using sofa::core::objectmodel::New;

    pybind11::class_<PythonController, BaseObject,
        PySofaBaseObject<PythonController>, // trampoline "alias" class 
        sofa::sptr<PythonController> >(m, "PythonController")
        .def(pybind11::init<>())
        .def("addCallback", &PythonController::addCallback)
        ;
}

}

}

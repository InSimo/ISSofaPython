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
        try
        {
           pybind11::gil_scoped_acquire acquire;
           it->second( e );
        }
        catch( const pybind11::error_already_set& e )
        {
            serr << "PythonController: Python exception in callback:\n" << e.what() << sendl;
        }
    }
}

void PythonController::cleanup()
{
    pybind11::gil_scoped_acquire gil;

    // The callback map is likely to hold function pointers that are member methods of a 
    // python class that derive from PythonController.
    // We therefore must clear it to ensure proper object destruction.
    m_callbackMap.clear();
}


void PythonController::addCallback(const sofa::core::objectmodel::Event* e, const HandleEventCallback& callback)
{
    m_callbackMap[std::string(e->getClassName())] = callback;
}

void PythonController::addCallback(const std::string& className, const HandleEventCallback& callback)
{
    m_callbackMap[className] = callback;
}

void PythonController::removeCallback(const sofa::core::objectmodel::Event* e)
{
    m_callbackMap.erase(std::string(e->getClassName()));
}

void PythonController::removeCallback(const std::string& className)
{
    m_callbackMap.erase(className);
}

void initBindingPythonController(pybind11::module& m)
{
    using sofa::core::objectmodel::BaseObject;
    using sofa::core::objectmodel::New;
    using sofa::python::PythonController;
    using HandleEventCallback = PythonController::HandleEventCallback;

    pybind11::class_<PythonController, BaseObject,
        PySofaBaseObject<PythonController>, // trampoline "alias" class 
        sofa::sptr<PythonController> >(m, "PythonController")
        .def(pybind11::init<>())
        .def("addCallback", pybind11::overload_cast<const sofa::core::objectmodel::Event*, const HandleEventCallback&>(&PythonController::addCallback))
        .def("addCallback", pybind11::overload_cast<const std::string&, const HandleEventCallback&>(&PythonController::addCallback))
        .def("removeCallback", pybind11::overload_cast<const sofa::core::objectmodel::Event*>(&PythonController::removeCallback))
        .def("removeCallback", pybind11::overload_cast<const std::string&>(&PythonController::removeCallback))
        ;
}

}

}

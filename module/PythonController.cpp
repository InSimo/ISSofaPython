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

#include "PythonController.h"
#include "BaseObjectBinding.h"

#include <sofa/core/ObjectFactory.h>

#include <pybind11/functional.h>
#include <ISSofaPython/GIL.h>
#include "common.h"

namespace sofa
{
namespace python
{

int PythonControllerClass = core::RegisterObject("Controller class to handle sofa events with python").add<PythonController>();

PythonController::PythonController()
:sofa::core::objectmodel::BaseObject()
{
    this->f_listening.setValue(true);
    //std::cout << "NEW PythonController @ " << (void*)this << std::endl;
    //std::cout << "  BaseObject @ " << (void*)(BaseObject*)this << std::endl;
    //std::cout << "  Base @ " << (void*)(Base*)this << std::endl;
    sofa::python::gil_scoped_acquire gil_acquire;
    try
    {
        setSourceLocation(this);
    }
    catch (const pybind11::error_already_set& e)
    {
        serr << "PythonController: Python exception in callback:\n" << e.what() << sendl;
    }

}

PythonController::~PythonController()
{

}

void PythonController::init()
{
    if (m_initCallback)
    {
        try
        {
            pybind11::gil_scoped_acquire acquire;
            m_initCallback();
        }
        catch (const pybind11::error_already_set& e)
        {
            serr << "PythonController: Python exception in init callback:\n" << sendl;
        }
       
    }
}


void PythonController::handleEvent(SofaEvent* e)
{
    auto it = m_callbackMap.find(std::string(e->getClassName()));

    if (it!= m_callbackMap.end())
    {
        try
        {
           sofa::python::gil_scoped_acquire gil_acquire;
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
    sofa::python::gil_scoped_acquire gil_acquire;
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

void PythonController::addInitCallback(const HandleVoidCallback& callback)
{
    m_initCallback = callback;
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
    using HandleVoidCallback = PythonController::HandleVoidCallback;

    pybind11::class_<PythonController, BaseObject,
        PySofaBaseObject<PythonController>, // trampoline "alias" class 
        sofa::sptr<PythonController> >(m, "PythonController", pybind11::multiple_inheritance())
        .def(pybind11::init<>())
        .def("addInitCallback", pybind11::overload_cast<const HandleVoidCallback&>(&PythonController::addInitCallback))
        .def("addCallback", pybind11::overload_cast<const sofa::core::objectmodel::Event*, const HandleEventCallback&>(&PythonController::addCallback))
        .def("addCallback", pybind11::overload_cast<const std::string&, const HandleEventCallback&>(&PythonController::addCallback))
        .def("removeCallback", pybind11::overload_cast<const sofa::core::objectmodel::Event*>(&PythonController::removeCallback))
        .def("removeCallback", pybind11::overload_cast<const std::string&>(&PythonController::removeCallback))
        ;
}

}

}

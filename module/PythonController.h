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

#ifndef ISSOFA_PYTHON_PYTHONCONTROLLER_H
#define ISSOFA_PYTHON_PYTHONCONTROLLER_H

#include <sofa/core/objectmodel/BaseObject.h>

#include <pybind11/pybind11.h>
#include <unordered_map>
#include <functional>

namespace sofa
{
namespace python
{


void initBindingPythonController(pybind11::module& m);


class PythonController : public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(PythonController, sofa::core::objectmodel::BaseObject);

    using SofaEvent = sofa::core::objectmodel::Event;

    void handleEvent(sofa::core::objectmodel::Event* ) override;

    void cleanup() override;

    typedef std::function<void(SofaEvent*)> HandleEventCallback;

    void addCallback(const sofa::core::objectmodel::Event*, const HandleEventCallback& callback);
    void addCallback(const std::string& className, const HandleEventCallback& callback);
    void removeCallback(const sofa::core::objectmodel::Event* e);
    void removeCallback(const std::string& className);

protected:
    PythonController();

    virtual ~PythonController();

    std::unordered_map< std::string, HandleEventCallback> m_callbackMap;

};

}

}

#endif // ! ISSOFA_PYTHON_PYTHONCONTROLLER_H

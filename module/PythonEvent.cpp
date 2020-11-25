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

#include "PythonEvent.h"
#include "EventBinding.h"

namespace sofa
{
namespace python
{


class PythonEvent : public sofa::core::objectmodel::Event
{
public:
    typedef sofa::core::objectmodel::Event SofaEvent;

    SOFA_EVENT_CLASS_EXTERNAL((PythonEvent), ((SofaEvent)));

    PythonEvent()
        :SofaEvent()
    {
    }

    virtual ~PythonEvent()
    {

    }

    const char* getClassName() const override
    {
        if (m_className.empty())
        {
            pybind11::object pyThis = pybind11::cast(this);
            auto handleType = pyThis.get_type();
            std::string fullTypeName = std::string(pybind11::str(handleType));
            auto start = fullTypeName.find_first_of('\'')+1;
            auto last = fullTypeName.find_last_of('\'');
            m_className = fullTypeName.substr(start, last-start);
        }
        return m_className.c_str();
    }
private:
    mutable std::string m_className;
};

SOFA_EVENT_CLASS_IMPL((PythonEvent));

void initBindingPythonEvent(pybind11::module& m)
{
    using sofa::core::objectmodel::Event;

    pybind11::class_<
        PythonEvent, Event,
        PyEvent <PythonEvent> 
                    >(m, "PythonEvent")
    .def(pybind11::init<>())
        ;
}

}


}
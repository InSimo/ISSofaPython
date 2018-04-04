/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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
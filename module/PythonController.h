/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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


// virtual inheritance fails when binding here.
class PythonController : public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(PythonController, sofa::core::objectmodel::BaseObject);

    using SofaEvent = sofa::core::objectmodel::Event;

    void handleEvent(sofa::core::objectmodel::Event* ) override;

    void cleanup() override;

    typedef std::function<void(SofaEvent*)> HandleEventCallback;

    void addCallback(sofa::core::objectmodel::Event*, const HandleEventCallback& callback);

protected:
    PythonController();

    virtual ~PythonController();

    std::unordered_map< std::string, HandleEventCallback> m_callbackMap;

};

}

}

#endif // ! ISSOFA_PYTHON_PYTHONCONTROLLER_H

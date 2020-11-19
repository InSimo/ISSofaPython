/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_PYTHONCONTROLLER_H
#define ISSOFA_PYTHON_PYTHONCONTROLLER_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/helper/system/thread/thread_specific_ptr.h>

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

    void addCallback(const sofa::core::objectmodel::Event*, const HandleEventCallback& callback);
    void addCallback(const std::string& className, const HandleEventCallback& callback);
    void removeCallback(const sofa::core::objectmodel::Event* e);
    void removeCallback(const std::string& className);

    /// A convenience class to obtain and release to GIL
    struct AcquireThreadState {
        AcquireThreadState() {
            if( have_lock == 0 ) {
                state = PyGILState_Ensure();
            }
            have_lock++;
        }

        virtual ~AcquireThreadState() {
            have_lock--;
            if( have_lock == 0 ) {
                PyGILState_Release( state );
            }
        }

        static SOFA_TLS_KEYWORD PyGILState_STATE state;
        static SOFA_TLS_KEYWORD int have_lock;
    };

protected:
    PythonController();

    virtual ~PythonController();

    std::unordered_map< std::string, HandleEventCallback> m_callbackMap;

};

}

}

#endif // ! ISSOFA_PYTHON_PYTHONCONTROLLER_H

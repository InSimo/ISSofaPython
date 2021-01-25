/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include <ISSofaPython/GIL.h>
#include <sofa/helper/system/thread/thread_specific_ptr.h>

namespace sofa
{
namespace python
{
namespace detail
{

namespace
{
SOFA_TLS_KEYWORD PyGILState_STATE state = PyGILState_STATE(0);
SOFA_TLS_KEYWORD int have_lock = 0;
}

AcquireThreadState::AcquireThreadState() 
{
    if (have_lock == 0) {
        state = PyGILState_Ensure();
    }
    have_lock++;
}

AcquireThreadState::~AcquireThreadState()
{
    have_lock--;
    if (have_lock == 0) {
        PyGILState_Release(state);
    }
}

}

}

}
/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFAPYTHON_SOFAGIL_H
#define ISSOFAPYTHON_SOFAGIL_H

#include <ISSofaPython/ISSofaPython.h>

namespace sofa
{
namespace python
{
namespace detail
{
/// A convenience class to obtain and release to GIL
/// May be required for versions of pybind11 below or equal to 2.4.3 
/// See also https://github.com/pybind/pybind11/issues/1364
struct ISSOFAPYTHON_API AcquireThreadState
{
    AcquireThreadState();

    ~AcquireThreadState();
};

}

#ifdef ISSOFAPYTHON_USE_PYBIND11_GIL
    using gil_scoped_acquire = pybind11::gil_scoped_acquire;
#else
    using gil_scoped_acquire = sofa::python::detail::AcquireThreadState;
#endif

}

}

#endif // ISSOFAPYTHON_SOFAGIL_H

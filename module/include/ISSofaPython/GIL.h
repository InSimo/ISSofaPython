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

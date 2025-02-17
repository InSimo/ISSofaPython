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
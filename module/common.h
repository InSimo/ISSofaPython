/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_COMMON_H
#define ISSOFA_PYTHON_COMMON_H

#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>

#include <pybind11/pybind11.h>

namespace sofa
{

namespace python
{

// Returns the most available Python derived type, depending on the C++ object,
// to allow the usage of specific methods in the Python side, for each object type.
pybind11::object getDerivedPyObject(sofa::core::objectmodel::Base* obj);

} // namespace python

} // namespace sofa

#endif // ISSOFA_PYTHON_COMMON_H
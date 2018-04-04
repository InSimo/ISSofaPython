/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_PLUGINMANAGERBINDING_H
#define ISSOFA_PYTHON_PLUGINMANAGERBINDING_H

#include <pybind11/pybind11.h>

namespace sofa
{
namespace python
{

void initBindingPluginManager(pybind11::module& m);

}

}

#endif // ISSOFA_PYTHON_PLUGINMANAGERBINDING_H

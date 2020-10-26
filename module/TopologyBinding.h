/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_TOPOLOGYBINDING_H
#define ISSOFA_PYTHON_TOPOLOGYBINDING_H

#include "BaseBinding.h"

#include <pybind11/pybind11.h>


namespace sofa
{

namespace python
{

void initBindingTopology(pybind11::module& m);

}


}


#endif // ISSOFA_PYTHON_TOPOLOGYBINDING_H

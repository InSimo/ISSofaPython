/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEMAPPINGBINDING_H
#define ISSOFA_PYTHON_BASEMAPPINGBINDING_H

#include "BaseBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/BaseMapping.h>

#include <pybind11/pybind11.h>

namespace sofa
{

namespace python
{

void initBindingBaseMapping(pybind11::module& m);

}

}

#endif // ISSOFA_PYTHON_BASEMAPPINGBINDING_H

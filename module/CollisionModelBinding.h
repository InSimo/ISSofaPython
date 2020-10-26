/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_COLLISIONMODELBINDING_H
#define ISSOFA_PYTHON_COLLISIONMODELBINDING_H

#include <pybind11/pybind11.h>
#include "BaseBinding.h"
#include "BaseObjectBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{

namespace python
{

void initBindingCollisionModel(pybind11::module& m);

}


}

#endif // ISSOFA_PYTHON_COLLISIONMODELBINDING_H

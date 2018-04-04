/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEOBJECTDESCRIPTIONBINDING_H
#define ISSOFA_PYTHON_BASEOBJECTDESCRIPTIONBINDING_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseObjectDescription.h>

namespace sofa
{

namespace python
{


void initBindingBaseObjectDescription(pybind11::module& m);

sofa::core::objectmodel::BaseObjectDescription createBaseObjectDescription(pybind11::args args, pybind11::kwargs kwargs);

}


}


#endif // !ISSOFA_PYTHON_BASEOBJECTDESCRIPTIONBINDING_H

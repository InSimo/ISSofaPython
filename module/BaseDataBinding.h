/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEDATABINDING_H
#define ISSOFA_PYTHON_BASEDATABINDING_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofa
{
namespace python
{

void initBindingBaseData(pybind11::module& m);

void setDataValueFromPyObject(sofa::core::objectmodel::BaseData* data, pybind11::object pyObj);


}

}
    
#endif // ISSOFA_PYTHON_BASEDATABINDING_H

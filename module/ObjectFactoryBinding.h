/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_OBJECTFACTORYBINDING_H
#define ISSOFA_PYTHON_OBJECTFACTORYBINDING_H

#include <pybind11/pybind11.h>

namespace sofa
{
namespace core
{
class ObjectFactory;

namespace objectmodel
{
class BaseContext;
}

}

namespace python
{

void initBindingObjectFactory(pybind11::module& m);


pybind11::object createObject(sofa::core::ObjectFactory* factory, sofa::core::objectmodel::BaseContext* ctx, pybind11::args args, pybind11::kwargs kwargs);

}

}

#endif // ISSOFA_PYTHON_OBJECTFACTORYBINDING_H

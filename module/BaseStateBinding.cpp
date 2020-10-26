/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseStateBinding.h"
#include <sofa/core/BaseState.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>


namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::BaseState;

void initBindingBaseState(pybind11::module& m)
{
    pybind11::class_<BaseState, BaseObject, Base,
                     PySofaBaseState<BaseState>,
                     sofa::sptr<BaseState>>(m, "BaseState")
        .def("resize", &BaseState::resize)
        ;
}

}

}

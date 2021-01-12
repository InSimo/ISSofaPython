/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "ContextBinding.h"
#include "BaseContextBinding.h"
#include <sofa/core/objectmodel/Context.h>

namespace sofa
{
namespace python
{

void initBindingContext(pybind11::module& m)
{
    using sofa::core::objectmodel::Context;
    using sofa::core::objectmodel::BaseContext;

    pybind11::class_<Context, BaseContext, 
                     PySofaBaseContextObject<Context>, sofa::sptr<Context> >(m, "Context", pybind11::multiple_inheritance());
}

}

}

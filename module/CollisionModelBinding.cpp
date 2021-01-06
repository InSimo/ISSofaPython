/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "CollisionModelBinding.h"

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/CollisionModel.h>


namespace sofa
{
namespace python
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;
using sofa::core::CollisionModel;

void initBindingCollisionModel(pybind11::module& m)
{
    pybind11::class_<CollisionModel, BaseObject,
                     sofa::sptr<CollisionModel>>(m, "CollisionModel")
        .def("setActive", &CollisionModel::setActive)
        ;
}

}

}

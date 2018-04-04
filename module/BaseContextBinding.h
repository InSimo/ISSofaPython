/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASECONTEXTBINDING_H
#define ISSOFA_PYTHON_BASECONTEXTBINDING_H

#include <pybind11/pybind11.h>
#include "BaseBinding.h"
#include <sofa/core/objectmodel/BaseObject.h>

namespace sofa
{

namespace python
{

void initBindingBaseContext(pybind11::module& m);


template< class TSofaContext >
class PySofaBaseContextObject : public PySofaBase<TSofaContext>
{
public:

    bool addObject(sofa::sptr<sofa::core::objectmodel::BaseObject> obj) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaContext, addObject, obj );
    }

    bool removeObject(sofa::sptr<sofa::core::objectmodel::BaseObject> obj ) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaContext, removeObject, obj );
    }

    const sofa::core::objectmodel::BaseClass* getClass() const override
    {
        PYBIND11_OVERLOAD(sofa::core::objectmodel::BaseClass*, TSofaContext, getClass, );
    }

    using PySofaBase<TSofaContext>::PySofaBase; // Inherit 
};

}

}

#endif // ISSOFA_PYTHON_BASECONTEXTBINDING_H

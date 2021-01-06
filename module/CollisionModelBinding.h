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

template< class TSofaObject >
class PySofaCollisionModel : public PySofaBaseObject< TSofaObject >
{
public:

    void isClassMacroUsed(TSofaObject* sofaObject) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, isClassMacroUsed, sofaObject);
    }

    void computeBoundingTree(int maxDepth=0) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, computeBoundingTree, maxDepth );
    }

    // const sofa::core::objectmodel::BaseContext* getContext() const override
    // {
    //     PYBIND11_OVERLOAD(const, sofa::core::objectmodel::BaseContext*, TSofaObject, getContext, const, );
    // }

    using PySofaBaseObject< TSofaObject >::PySofaBaseObject; // Inherit constructors

};
}


}

#endif // ISSOFA_PYTHON_COLLISIONMODELBINDING_H

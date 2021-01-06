/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASESTATEBINDING_H
#define ISSOFA_PYTHON_BASESTATEBINDING_H

#include "BaseBinding.h"

#include <pybind11/pybind11.h>

namespace sofa
{

namespace python
{

void initBindingBaseState(pybind11::module& m);

template< class TSofaObject >
class PySofaBaseState : public PySofaBase<TSofaObject>
{
public:

    void isClassMacroUsed(TSofaObject* sofaObject) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, isClassMacroUsed, sofaObject);
    }
    
    int getSize() const override
    {
        PYBIND11_OVERLOAD_PURE(int, TSofaObject, getSize, );
    }

    void resize(int vsize) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, resize, vsize);
    }

    sofa::core::objectmodel::BaseData* baseWrite(sofa::core::VecId v) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::core::objectmodel::BaseData*, TSofaObject, baseWrite, v);
    }

    const sofa::core::objectmodel::BaseData* baseRead(sofa::core::ConstVecId v) const override
    {
        PYBIND11_OVERLOAD_PURE(const sofa::core::objectmodel::BaseData*, TSofaObject, baseRead, v);
    }

    using PySofaBase<TSofaObject>::PySofaBase; // Inherit constructors
};

}


}


#endif // ISSOFA_PYTHON_BASESTATEBINDING_H

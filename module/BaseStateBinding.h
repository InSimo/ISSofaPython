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

    void resize(int vsize) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, resize, vsize);
    }

    using PySofaBase<TSofaObject>::PySofaBase; // Inherit constructors
};

}


}


#endif // ISSOFA_PYTHON_BASESTATEBINDING_H

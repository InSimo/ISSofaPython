/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEBINDING_H
#define ISSOFA_PYTHON_BASEBINDING_H

#include <ISSofaPython/ISSofaPython.h>
#include <sofa/core/objectmodel/BaseClass.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/simulation/tree/GNode.h>

namespace sofa
{

namespace python
{


void initBindingBase(pybind11::module& m);


template< class TSofaBase >
class PySofaBase : public TSofaBase
{
public:

    const sofa::core::objectmodel::BaseClass* getClass() const override
    {
        PYBIND11_OVERLOAD_PURE(sofa::core::objectmodel::BaseClass*, TSofaBase, getClass, );
    }

    using TSofaBase::TSofaBase; // Inherit constructors
};


}


}

#endif // ISSOFA_PYTHON_BASEBINDING_H

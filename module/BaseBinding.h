/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEBINDING_H
#define ISSOFA_PYTHON_BASEBINDING_H

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseClass.h>
#include <boost/intrusive_ptr.hpp>

namespace sofa
{

template< typename T>
using sptr = boost::intrusive_ptr<T>;

}

PYBIND11_DECLARE_HOLDER_TYPE(T, sofa::sptr<T>, true);


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

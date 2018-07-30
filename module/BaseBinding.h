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
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/simulation/tree/GNode.h>

namespace sofa
{

template< typename T>
using sptr = boost::intrusive_ptr<T>;

}

PYBIND11_DECLARE_HOLDER_TYPE(T, sofa::sptr<T>, true)


namespace sofa
{

namespace python
{

void initBindingBase(pybind11::module& m);

pybind11::object bindDataAndLinks(sofa::sptr<core::objectmodel::BaseObject> obj);

pybind11::object bindDataAndLinks(sofa::sptr<simulation::tree::GNode> obj);

void genericBindDataAndLinks(pybind11::object& pyObj, const helper::vector<core::objectmodel::BaseData*> dataVec,
    const helper::vector<BaseLink*> linkVec, const std::string& objName);

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

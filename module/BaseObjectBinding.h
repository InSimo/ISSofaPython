/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEOBJECTBINDING_H
#define ISSOFA_PYTHON_BASEOBJECTBINDING_H

#include <pybind11/pybind11.h>
#include "BaseBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofa
{

namespace python
{

std::string getPath(const sofa::core::objectmodel::BaseObject* obj);

void initBindingBaseObject(pybind11::module& m);


template< class TSofaObject >
class PySofaBaseObject : public PySofaBase<TSofaObject>
{
public:

    const sofa::core::objectmodel::BaseClass* getClass() const override
    {
        PYBIND11_OVERLOAD(sofa::core::objectmodel::BaseClass*, TSofaObject, getClass, );
    }
    
    void init() override 
    {
        PYBIND11_OVERLOAD(void, TSofaObject, init, );
    }

    void bwdInit() override
    {
        PYBIND11_OVERLOAD(void, TSofaObject, bwdInit, );
    }

    void reinit() override
    {
        PYBIND11_OVERLOAD(void, TSofaObject, reinit, );
    }

    void cleanup() override
    {
        PYBIND11_OVERLOAD(void, TSofaObject, cleanup, );
    }

    void reset() override
    {
        PYBIND11_OVERLOAD(void, TSofaObject, reset, );
    }

    void draw(const core::visual::VisualParams* vparams) override
    {
        PYBIND11_OVERLOAD(void, TSofaObject, draw, vparams);
    }

    using PySofaBase<TSofaObject>::PySofaBase; // Inherit constructors
};

}


}


#endif // ISSOFA_PYTHON_BASEOBJECTBINDING_H

/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_BASEMAPPINGBINDING_H
#define ISSOFA_PYTHON_BASEMAPPINGBINDING_H

#include "BaseObjectBinding.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/BaseMapping.h>

#include <pybind11/pybind11.h>
#include <string>

namespace sofa
{

namespace python
{

using sofa::core::MechanicalParams;
using sofa::core::ConstraintParams;
using sofa::core::BaseState;

using sofa::core::MultiVecCoordId;
using sofa::core::VecCoordId;
using sofa::core::ConstMultiVecCoordId;
using sofa::core::ConstVecCoordId;

using sofa::core::MultiVecDerivId;
using sofa::core::VecDerivId;
using sofa::core::ConstMultiVecDerivId;
using sofa::core::ConstVecDerivId;
using sofa::core::MultiMatrixDerivId;
using sofa::core::ConstMultiMatrixDerivId;

void initBindingBaseMapping(pybind11::module& m);

template< class TSofaObject >
class PySofaBaseMapping : public PySofaBaseObject< TSofaObject >
{
public:

    const sofa::core::objectmodel::BaseClass* getClass() const override
    {
        PYBIND11_OVERLOAD(sofa::core::objectmodel::BaseClass*, TSofaObject, getClass, );
    }

    void isClassMacroUsed(TSofaObject* sofaObject) override 
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, isClassMacroUsed, sofaObject);
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


    void apply(const MechanicalParams* mparams = MechanicalParams::defaultInstance(), MultiVecCoordId outPos = VecCoordId::position(), ConstMultiVecCoordId inPos = ConstVecCoordId::position() ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, apply, mparams, outPos, inPos );
    }

    void applyJ(const MechanicalParams* mparams = MechanicalParams::defaultInstance(), MultiVecDerivId outVel = VecDerivId::velocity(), ConstMultiVecDerivId inVel = ConstVecDerivId::velocity() ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, applyJ, mparams, outVel, inVel );
    }

    sofa::helper::vector<BaseState*> getFrom( ) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::helper::vector<BaseState*>, TSofaObject, getFrom, );
    }

    bool setFrom(BaseState* from ) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaObject, setFrom, from );
    }

    sofa::helper::vector<BaseState*> getTo( ) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::helper::vector<BaseState*>, TSofaObject, getTo, );
    }

    bool setTo(BaseState* to ) override
    {
        PYBIND11_OVERLOAD_PURE(bool, TSofaObject, setTo, to );
    }

    void applyJT(const MechanicalParams* mparams, MultiVecDerivId inForce, ConstMultiVecDerivId outForce ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, applyJT, mparams, inForce, outForce );
    }

    void applyDJT(const MechanicalParams* mparams, MultiVecDerivId inForce, ConstMultiVecDerivId outForce ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, applyDJT, mparams, inForce, outForce );
    }

    void applyJT(const ConstraintParams* mparams, MultiMatrixDerivId inConst, ConstMultiMatrixDerivId outConst ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, applyJT, mparams, inConst, outConst );
    }

    void computeAccFromMapping(const MechanicalParams* mparams, MultiVecDerivId outAcc, ConstMultiVecDerivId inVel, ConstMultiVecDerivId inAcc ) override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, computeAccFromMapping, mparams, outAcc, inVel, inAcc );
    }

    sofa::helper::vector<sofa::core::behavior::BaseMechanicalState*> getMechFrom( ) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::helper::vector<sofa::core::behavior::BaseMechanicalState*>, TSofaObject, getMechFrom, );
    }

    sofa::helper::vector<sofa::core::behavior::BaseMechanicalState*> getMechTo( ) override
    {
        PYBIND11_OVERLOAD_PURE(sofa::helper::vector<sofa::core::behavior::BaseMechanicalState*>, TSofaObject, getMechTo, );
    }

    void disable() override
    {
        PYBIND11_OVERLOAD_PURE(void, TSofaObject, disable, );
    }

    using PySofaBaseObject< TSofaObject >::PySofaBaseObject; // Inherit constructors

};
}

}

#endif // ISSOFA_PYTHON_BASEMAPPINGBINDING_H

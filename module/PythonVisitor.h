/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_PYTHONVISITOR_H
#define ISSOFA_PYTHON_PYTHONVISITOR_H

#include <pybind11/pybind11.h>

#include <sofa/simulation/common/Visitor.h>

namespace sofa
{

namespace simulation
{


class PythonVisitor : public Visitor
{
public:
    PythonVisitor(const core::ExecParams* params, pybind11::object& pyVisitor);

    virtual Result processNodeTopDown(simulation::Node* node);
    virtual void processNodeBottomUp(simulation::Node* node);

    /// It seems no possible to pass a reference variable to a python function, so repeat cannot be modified in python
    /// The python function must returns a code:
    /// -1 -> dag / NO_REPETITION
    ///  0 -> tree / NO_REPETITION
    ///  1 -> tree / REPEAT_ONCE
    ///  2 -> tree / REPEAT_ALL
    virtual bool treeTraversal(TreeTraversalRepetition& repeat);

protected:
    pybind11::object m_PyVisitor;
};


} // namespace simulation

} // namespace sofa

#endif // ISSOFA_PYTHON_PYTHONVISITOR_H

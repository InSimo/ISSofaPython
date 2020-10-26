/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PythonVisitor.h"


namespace sofa
{

namespace simulation
{


PythonVisitor::PythonVisitor(const core::ExecParams* params, pybind11::object& pyVisitor)
    : Visitor(params)
    , m_PyVisitor(pyVisitor)
{
}

Visitor::Result PythonVisitor::processNodeTopDown(simulation::Node* node)
{
    pybind11::object res = m_PyVisitor.attr("processNodeTopDown")(pybind11::cast(node));
    if( res.is_none() || !pybind11::isinstance<pybind11::bool_>(res) )
    {
        // no error -> the function is not overloaded -> default implementation
        return Visitor::RESULT_CONTINUE;
    }

    bool r = ( res == Py_True );
    return r ? Visitor::RESULT_CONTINUE : Visitor::RESULT_PRUNE;
}

void PythonVisitor::processNodeBottomUp(simulation::Node* node)
{
    pybind11::object res = m_PyVisitor.attr("processNodeBottomUp")(pybind11::cast(node));
}

bool PythonVisitor::treeTraversal(TreeTraversalRepetition& repeat)
{
    pybind11::object res = m_PyVisitor.attr("treeTraversal")();

    if( res.is_none() || !pybind11::isinstance<pybind11::int_>(res) )
    {
        // no error -> the function is not overloaded -> default implementation
        repeat = NO_REPETITION;
        return false;
    }

    int r = res.cast<int>();
    switch( r )
    {
        case 0: // tree no repeat
            repeat = NO_REPETITION;
            return true;
            break;
        case 1: // tree repeat once
            repeat = REPEAT_ONCE;
            return true;
            break;
        case 2: // tree repeat all
            repeat = REPEAT_ALL;
            return true;
            break;
        default:
        case -1: // dag
            repeat = NO_REPETITION;
            return false;
    }

}

} // namespace simulation

} // namespace sofa

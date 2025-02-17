/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*            (c) 2006-2021 MGH, INRIA, USTL, UJF, CNRS, InSimo                *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

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

    bool r = res.cast<bool>();
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

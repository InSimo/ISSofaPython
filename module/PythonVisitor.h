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

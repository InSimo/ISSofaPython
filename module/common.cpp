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

#include "common.h"

#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/Context.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/tree/GNode.h>

#include <sofa/core/topology/Topology.h>
#include <sofa/core/topology/BaseMeshTopology.h>

#include <sofa/core/BaseState.h>
#include <sofa/core/BaseMapping.h>
#include <sofa/core/CollisionModel.h>

#include <SofaBaseTopology/PointSetTopologyModifier.h>
#include <SofaBaseTopology/TriangleSetTopologyModifier.h>

#include "PythonController.h"

namespace sofa
{

namespace python
{

pybind11::object getDerivedPyObject(sofa::core::objectmodel::Base* obj)
{
    if (auto derived = sofa::simulation::tree::GNode::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::simulation::Node::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::core::objectmodel::Context::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::core::objectmodel::BaseContext::DynamicCast(obj))
        return pybind11::cast(derived);
    if (auto derived = sofa::core::objectmodel::BaseNode::DynamicCast(obj))
        return pybind11::cast(derived);

    if (auto derived = PythonController::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }

    if (auto derived = sofa::core::topology::BaseMeshTopology::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::topology::Topology::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }

    if (auto derived = sofa::core::BaseState::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::BaseMapping::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::CollisionModel::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::component::topology::TriangleSetTopologyModifier::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::component::topology::PointSetTopologyModifier::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }
    if (auto derived = sofa::core::objectmodel::BaseObject::DynamicCast(obj)) {
        return pybind11::cast(derived);
    }

    return pybind11::cast(obj);
}

void setSourceLocation(sofa::core::objectmodel::Base* obj)
{
    // store the source location of this object
    //PyFrameObject* frame = PyEval_GetFrame();
    PyThreadState *tstate = PyThreadState_GET();
    if (NULL != tstate && NULL != tstate->frame)
    {
        PyFrameObject *frame = tstate->frame;
        int depth = 6; // max number of frames to capture
        do
        {
            // int line = frame->f_lineno;
            /*
            frame->f_lineno will not always return the correct line number
            you need to call PyCode_Addr2Line().
            */
            int line = PyCode_Addr2Line(frame->f_code, frame->f_lasti);
            std::string filename;
            // https://stackoverflow.com/questions/22487780/what-do-i-use-instead-of-pystring-asstring-when-loading-a-python-module-in-3-3
            if (PyUnicode_Check(frame->f_code->co_filename)) {
                PyObject* tempBytes = PyUnicode_AsEncodedString(frame->f_code->co_filename, "UTF-8", "strict");
                filename = PYBIND11_BYTES_AS_STRING(tempBytes);
                Py_DECREF(tempBytes);
            }
            else if (PyBytes_Check(frame->f_code->co_filename)) {
                filename = PYBIND11_BYTES_AS_STRING(frame->f_code->co_filename);
            }
            //const char *funcname = PyString_AsString(frame->f_code->co_name);
            //printf("    %s(%d): %s\n", filename, line, funcname);
            obj->addSourceFile(filename.c_str(), line, 0);
            frame = frame->f_back;
        } while (NULL != frame && --depth > 0);
    }

}

} // namespace python

} // namespace sofa

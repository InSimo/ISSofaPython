/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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

/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "ISSofaPythonModule.h"

#include "BaseClassBinding.h"
#include "BaseObjectDescriptionBinding.h"
#include "BaseDataBinding.h"
#include "BaseLinkBinding.h"
#include "BaseBinding.h"
#include "EventBinding.h"
#include "BaseObjectBinding.h"
#include "BaseContextBinding.h"
#include "ContextBinding.h"
#include "BaseNodeBinding.h"
#include "ObjectFactoryBinding.h"

#include "NodeBinding.h"
#include "GNodeBinding.h"

#include "PythonController.h"
#include "PythonEvent.h"
#include "PluginManagerBinding.h"

#include <sofa/simulation/common/Simulation.h>
#include <sofa/simulation/tree/GNode.h>

PYBIND11_MODULE(ISSofaPython, m)
{

    using sofa::simulation::tree::GNode;

    sofa::python::initBindingPluginManager(m);

    sofa::python::initBindingBaseClassInfo(m);
    sofa::python::initBindingBaseClass(m);
    sofa::python::initBindingEventClass(m);

    sofa::python::initBindingBaseObjectDescription(m);
    sofa::python::initBindingBaseLink(m);
    sofa::python::initBindingBaseData(m);
    sofa::python::initBindingBase(m);
    sofa::python::initBindingEvent(m);
    sofa::python::initBindingBaseObject(m);
    sofa::python::initBindingBaseContext(m);
    sofa::python::initBindingContext(m);
    sofa::python::initBindingBaseNode(m);
    sofa::python::initBindingObjectFactory(m);

    sofa::python::initBindingNode(m);
    sofa::python::initBindingGNode(m);

    sofa::python::initBindingPythonController(m);
    sofa::python::initBindingPythonEvent(m);

    m.def("initializeGraph", [](GNode::SPtr node) { sofa::simulation::initializeGraph(node.get()); });
    m.def("cleanupGraph", [](GNode::SPtr node) { sofa::simulation::cleanupGraph(node.get());  });
    m.def("step", [](GNode::SPtr node, double dt) {
        
        sofa::core::ExecParams* params = sofa::core::ExecParams::defaultInstance();
        sofa::core::behavior::BaseAnimationLoop* aloop = node->getAnimationLoop();
        if (aloop)
        {
            aloop->step(params, dt);
        }
    });
}
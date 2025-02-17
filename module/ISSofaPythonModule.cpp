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

#include "ISSofaPythonModule.h"

#include "BaseClassBinding.h"
#include "BaseObjectDescriptionBinding.h"
#include "BaseDataBinding.h"
#include "BaseLinkBinding.h"
#include "BaseBinding.h"
#include "EventBinding.h"
#include "BaseObjectBinding.h"
#include "BaseMappingBinding.h"
#include "CollisionModelBinding.h"
#include "BaseContextBinding.h"
#include "ContextBinding.h"
#include "BaseNodeBinding.h"
#include "ObjectFactoryBinding.h"
#include "BaseStateBinding.h"

#include "NodeBinding.h"
#include "GNodeBinding.h"

#include "TopologyBinding.h"
#include "BaseMeshTopologyBinding.h"
#include "PointSetTopologyModifierBinding.h"
#include "TriangleSetTopologyModifierBinding.h"

#include "PythonController.h"
#include "PythonEvent.h"
#include "PluginManagerBinding.h"

#include "VisualParamsBinding.h"
#include "DrawToolBinding.h"

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
    sofa::python::initBindingBaseState(m);
    sofa::python::initBindingBaseMapping(m);
    sofa::python::initBindingCollisionModel(m);
    sofa::python::initBindingBaseContext(m);
    sofa::python::initBindingContext(m);
    sofa::python::initBindingBaseNode(m);
    sofa::python::initBindingObjectFactory(m);

    sofa::python::initBindingTopology(m);
    sofa::python::initBindingBaseMeshTopology(m);
    sofa::python::initBindingPointSetTopologyModifier(m);
    sofa::python::initBindingTriangleSetTopologyModifier(m);

    sofa::python::initBindingNode(m);
    sofa::python::initBindingGNode(m);

    sofa::python::initBindingPythonController(m);
    sofa::python::initBindingPythonEvent(m);

    sofa::python::initBindingVisualParams(m);
    sofa::python::initBindingDrawTool(m);

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
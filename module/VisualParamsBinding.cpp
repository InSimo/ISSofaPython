/*******************************************************************************
*          Private SOFA components, (c) 2020 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "VisualParamsBinding.h"
#include <sofa/core/visual/VisualParams.h>

namespace sofa
{
namespace python
{

using sofa::core::visual::VisualParams;


void initBindingVisualParams(pybind11::module& m)
{
    pybind11::class_<VisualParams>(m, "VisualParams")
        .def_property_readonly("drawTool", pybind11::overload_cast<>(&VisualParams::drawTool, pybind11::const_));
}

}

}

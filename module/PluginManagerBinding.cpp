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

#include "PluginManagerBinding.h"
#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/PluginManager.h>
#include <pybind11/iostream.h>

namespace sofa
{
namespace python
{

using sofa::helper::system::Plugin;
using sofa::helper::system::PluginManager;

void findPlugin(PluginManager* pluginManager, std::string& path, const std::string& suffix = PluginManager::getDefaultSuffix())
{
    pluginManager->findPlugin(path, suffix);
}

bool hasPlugin(PluginManager* pluginManager, std::string& path, bool finalPath = false)
{
    return pluginManager->hasPlugin(path, finalPath);
}

void loadPlugin(PluginManager* pluginManager, const std::string& path, bool finalPath = false)
{
    std::string pathCopy = path;
    if (pluginManager->hasPlugin(pathCopy, finalPath))
    {
        return;
    }

    const bool loaded = pluginManager->loadPlugin(pathCopy, &std::cerr, false);
    if (!loaded)
    {
        throw std::invalid_argument("loadPlugin failed with " + path);
    }
}

void pluginManagerLoadPlugin(const std::string& path, bool finalPath = false)
{
    std::string pathCopy = path;
    loadPlugin(&PluginManager::getInstance(), path, finalPath);    
}

void pluginManagerAddDataPath(const std::string& path)
{
    sofa::helper::system::DataRepository.addFirstPath(path);
}

void initBindingPluginManager(pybind11::module& m)
{
    pybind11::class_<PluginManager, std::unique_ptr<PluginManager, pybind11::nodelete> >(m, "PluginManager")
        .def_static("getInstance", &PluginManager::getInstance,
            pybind11::return_value_policy::reference)
        .def_static("getDefaultSuffix", &PluginManager::getDefaultSuffix)
        .def("findPlugin", &findPlugin, pybind11::arg("path"), pybind11::arg("suffix") = PluginManager::getDefaultSuffix())
        .def("hasPlugin", &hasPlugin, pybind11::arg("path"), pybind11::arg("finalPath") = false)
        .def("loadPlugin", &loadPlugin, pybind11::arg("path"), pybind11::arg("finalPath") = false)
        ;

    m.def("loadPlugin", &pluginManagerLoadPlugin, pybind11::arg("path"), pybind11::arg("finalPath") = false);
    m.def("addDataPath", &pluginManagerAddDataPath, pybind11::arg("path"));

}

}


}

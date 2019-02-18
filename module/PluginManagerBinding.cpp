/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

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

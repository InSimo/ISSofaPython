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

#include "PythonSceneLoader.h"
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/simulation/tree/GNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#include <pybind11/stl.h> // to cast std::vector<std::string> into list<str>
#include <pybind11/eval.h>
#include <ISSofaPython/ISSofaPython.h>
#include <ISSofaPython/GIL.h>

namespace sofa
{
namespace python
{

using sofa::simulation::SceneLoaderFactory;
using sofa::simulation::SceneLoader;

PythonSceneLoader::PythonSceneLoader()
{
    sofa::python::gil_scoped_acquire acquire;

    auto sys = pybind11::module::import("sys");
    std::vector<std::string> localPath;
#ifdef _WIN32
    localPath.push_back(sofa::helper::system::SetDirectory::GetRelativeFromProcess("."));
#else
    localPath.push_back(sofa::helper::system::SetDirectory::GetRelativeFromProcess("../lib"));
#endif
    if (!localPath.empty())
    {
        auto syspath = sys.attr("path").cast<pybind11::list>();
        for (const std::string& s: localPath)
        {
            // Append the path to sys.path, if not already there
            if (!syspath.contains(s))
            {
                std::cout << "ISSofaPython: adding to python path: " << s << std::endl;
                syspath.append(s);
            }
        }
    }
#ifdef ISSOFAPYTHON_DONT_WRITE_BYTECODE
    sys.attr("dont_write_bytecode") = true;
#endif
}

PythonSceneLoader::~PythonSceneLoader()
{
}

bool PythonSceneLoader::canLoadFileExtension(const char *extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext=="py");
}

std::string PythonSceneLoader::getFileTypeDesc()
{
    return "Python Scenes";
}

void PythonSceneLoader::getExtensionList(ExtensionList* list)
{
    list->clear();
    list->push_back("py");
}

sofa::simulation::Node::SPtr PythonSceneLoader::load(const char *filename, const SceneArguments& sceneArguments)
{
    using sofa::helper::system::SetDirectory;
    using sofa::simulation::tree::GNode;
    using sofa::core::objectmodel::New;

#if !defined(WIN32)
    // Reset locale settings to make sure that floating-point values are interpreted correctly
    setlocale(LC_ALL,"C");
    setlocale(LC_NUMERIC,"C");
#endif

    std::string fileDir = SetDirectory::GetParentDir(filename);
    std::string file    = SetDirectory::GetFileNameWithoutExtension(filename);

    sofa::helper::system::DataRepository.addFirstPath(fileDir);
        
    auto node = sofa::simulation::tree::getSimulation()->createNewGraph("root");
    GNode::SPtr gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);

    static std::string prevSceneDir = "";

    sofa::python::gil_scoped_acquire acquire;

    try
    {
        pybind11::module sys = pybind11::module::import("sys");
        if (!prevSceneDir.empty())
        {
            std::cout << "Cleaning previous scene:" << std::endl;

            // Remove the previous scene directory from sys.path so that new modules imports
            // are not searched in it
            std::cout << "  - remove '" << prevSceneDir << "' from sys.path" << std::endl;
            sys.attr("path").attr("remove")(prevSceneDir);

            // In case the previous and new scenes dirs contain local python modules having
            // identical names, we delete any imported module residing in the previous scene dir
            // from sys.modules, to ensure new potential imports of the same modules names do not
            // return already-imported modules residing next to the previous scene.
            // We want new imports to load the modules residing next to the new scene.
            pybind11::object scope = pybind11::globals();
            scope["PREV_SCENE_DIR"] = prevSceneDir;
            scope["SCENE_FILE_PATH"] = filename;
            pybind11::exec(R"(
                import sys
                from os import path
                prev_scene_local_modules = []
                # SCENE_FILE_NAME = path.basename(SCENE_FILE_PATH)
                for name, module in sys.modules.items():
                    module_file_path = getattr(module, '__file__', None)
                    if module_file_path is None:
                        continue
                    # Uncomment the following lines if you want to avoid a reload of the python module
                    # of the scene when doing a 'reload' from the GUI (reloading is convenient when developing)
                    # if (module_file_path.endswith(SCENE_FILE_NAME) or
                    #         module_file_path.endswith(SCENE_FILE_NAME + 'c')):
                    #     p1 = path.abspath(SCENE_FILE_PATH if not SCENE_FILE_PATH.endswith('c')
                    #                       else SCENE_FILE_PATH[:-1])
                    #     p2 = path.abspath(module_file_path if not module_file_path.endswith('c')
                    #                       else module_file_path[:-1])
                    #     if p1 == p2:
                    #         continue  # the scene module was already imported and won't be re-read
                    if module_file_path.startswith(PREV_SCENE_DIR):
                        prev_scene_local_modules.append(name)
                for name in prev_scene_local_modules:
                    print('  - delete %r from sys.modules' % name)
                    del sys.modules[name]
                )",
                scope
            );
        }

        if (!fileDir.empty())
        {
            sys.attr("path").cast<pybind11::list>().append(fileDir);
        }
        prevSceneDir = fileDir;

        sys.attr("argv") = sceneArguments;

        auto fileModule = pybind11::module::import(file.c_str());
        fileModule.attr("createScene")(gNode);
    }
    catch (pybind11::error_already_set& e)
    {
        std::cerr << "Python Exception:" << std::endl;
        std::cerr << e.what() << std::endl;
        e.restore();
    }

    return gNode;
}

}

}

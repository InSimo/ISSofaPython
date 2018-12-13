/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PythonSceneLoader.h"
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/DynamicLibrary.h>
#include <sofa/simulation/tree/GNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#ifdef PYTHON_LIBRARY_SONAME
#include <dlfcn.h>
#endif
#include <pybind11/stl.h> // to cast std::vector<std::string> into list<str>

namespace sofa
{

template< typename T>
using sptr = boost::intrusive_ptr<T>;

}

PYBIND11_DECLARE_HOLDER_TYPE(T, sofa::sptr<T>, true)


namespace sofa
{
namespace python
{

using sofa::simulation::SceneLoaderFactory;
using sofa::simulation::SceneLoader;

PythonSceneLoader::PythonSceneLoader()
{
    // On Linux when using a virtualenv, native libraries such as used by numpy fail to load unless the python library is manually loaded
#ifdef PYTHON_LIBRARY_SONAME
    std::cout << "ISSofaPython: loading python library: " << PYTHON_LIBRARY_SONAME << std::endl;
    sofa::helper::system::DynamicLibrary::DynamicLibrary::load(PYTHON_LIBRARY_SONAME, true);
#endif
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
            std::cout << "ISSofaPython: adding to python path: " << s << std::endl;
            syspath.append(s);
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

    std::string fileDir = SetDirectory::GetParentDir(filename);
    std::string file    = SetDirectory::GetFileNameWithoutExtension(filename);

    sofa::helper::system::DataRepository.addFirstPath(fileDir);
        
    auto node = sofa::simulation::tree::getSimulation()->createNewGraph("root");
    GNode::SPtr gNode = sofa::core::objectmodel::SPtr_static_cast<GNode>(node);

    try
    {
        pybind11::module sys = pybind11::module::import("sys");
        sys.attr("path").cast<pybind11::list>().append(fileDir);

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
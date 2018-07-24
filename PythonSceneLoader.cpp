/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PythonSceneLoader.h"
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/FileSystem.h>
#include <sofa/simulation/tree/GNode.h>
#include <sofa/simulation/tree/TreeSimulation.h>

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
    pybind11::initialize_interpreter();
    // Activation of local virtualenv paths, if available
    std::string prefix = sofa::helper::system::SetDirectory::GetRelativeFromProcess("../");
    // Handle case where executables are in <builddir>/bin/<config>
    if (sofa::helper::system::SetDirectory::GetFileName(prefix.c_str()) == "bin")
    {
        prefix = sofa::helper::system::SetDirectory::GetParentDir(prefix.c_str());
    }
    auto sys = pybind11::module::import("sys");

    std::string site_packages = prefix;
#ifdef _WIN32
    site_packages += "Lib/site-packages";
#else
    std::string sysversion = sys.attr("version").cast<pybind11::str>();
    site_packages += "lib/python";
    site_packages += sysversion.substr(0,3);
    site_packages += "/site-packages";
#endif
    if (sofa::helper::system::FileSystem::exists(site_packages) &&
        sofa::helper::system::FileSystem::isDirectory(site_packages))
    {
        std::cout << "ISSofaPython: setting prefix: " << prefix << std::endl;
        sys.attr("prefix") = prefix;
        std::cout << "ISSofaPython: addsitedir: " << site_packages << std::endl;
        pybind11::module::import("site").attr("addsitedir")(site_packages);
    }
    std::vector<std::string> localPath;
#ifdef _WIN32
    localPath.push_back(sofa::helper::system::SetDirectory::GetRelativeFromProcess("."));
#else
    localPath.push_back(sofa::helper::system::SetDirectory::GetRelativeFromProcess("../lib"));
#endif
    if (!localPath.empty())
    {
        auto syspath = pybind11::module::import("sys").attr("path").cast<pybind11::list>();
        for (const std::string& s: localPath)
        {
            std::cout << "ISSofaPython: adding to python path: " << s << std::endl;
            syspath.append(s);
        }
    }
}

PythonSceneLoader::~PythonSceneLoader()
{
    // we will never reach that point... 
    // SceneLoaderFactory bleeds memory, and SceneLoader has no virtual destructor...
    pybind11::finalize_interpreter(); 
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


sofa::simulation::Node::SPtr PythonSceneLoader::load(const char* filename)
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

        auto fileModule = pybind11::module::import(file.c_str());

        auto createScene = fileModule.attr("createScene");

        pybind11::object pyGNode = pybind11::cast(gNode);

        pybind11::object result  = createScene(pyGNode);
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

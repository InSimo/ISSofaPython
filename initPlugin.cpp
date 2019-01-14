/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "initPlugin.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/FileSystem.h>
#include "PythonSceneLoader.h"

// Not including <Python.h> here because the include of <pybind11/embed.h>
// does it, and pybind also ensures there is no trouble when doing a debug build
// (use of the release python lib instead of the debug one which is not
// directly shipped with python)
#include <pybind11/embed.h>

namespace sofa
{

namespace python
{

sofa::helper::Creator<sofa::simulation::SceneLoaderFactory, PythonSceneLoader> creatorPythonSceneLoader("PythonSceneLoader", false, 0, "Scene Loader for py simulation files", { "py" });

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
    ISSOFAPYTHONPLUGIN_API void initExternalModule();
    ISSOFAPYTHONPLUGIN_API const char* getModuleName();
    ISSOFAPYTHONPLUGIN_API const char* getModuleVersion();
    ISSOFAPYTHONPLUGIN_API const char* getModuleLicense();
    ISSOFAPYTHONPLUGIN_API const char* getModuleDescription();
    ISSOFAPYTHONPLUGIN_API const char* getModuleComponentList();
}

void initExternalModule()
{
    if (Py_IsInitialized())
    {
        // The Python interpreter is certainly already initialized by H3D
        return;
    }

    // Let's initialize the Python interpreter, possibly using the local virtual env
#ifdef ISSOFAPYTHON_USE_VIRTUALENV
    // using static here because we need a static storage for the call to Py_SetPythonHome
    static std::string virtualenvDir;

    // In an installed/packaged context, we put the local python in a subdirectory of the binaries,
    // so let's try this first
    std::string potentialVenvDir = sofa::helper::system::SetDirectory::GetRelativeFromProcess(ISSOFAPYTHON_VIRTUALENV_DIRNAME);
    if (sofa::helper::system::FileSystem::exists(potentialVenvDir.c_str()) &&
        sofa::helper::system::FileSystem::isDirectory(potentialVenvDir.c_str()))
    {
        std::cout << "ISSofaPython: using local python in " << potentialVenvDir << std::endl;
        virtualenvDir = potentialVenvDir;
    }
    // We may have more luck with the local python of the build tree, if still reachable
    else if (sofa::helper::system::FileSystem::exists(ISSOFAPYTHON_VIRTUALENV_DIR) &&
             sofa::helper::system::FileSystem::isDirectory(ISSOFAPYTHON_VIRTUALENV_DIR))
    {
        std::cout << "ISSofaPython: using ISSOFAPYTHON_VIRTUALENV_DIR " << ISSOFAPYTHON_VIRTUALENV_DIR << std::endl;
        virtualenvDir = ISSOFAPYTHON_VIRTUALENV_DIR;
    }
    // The build tree is certainly not reachable anymore, or may have been moved so
    // let's search relatively to the current process dir (this case is relevant if
    // we execute the binaries of the build tree after having moved things)
    else
    {
        std::cout << "WARNING: ISSofaPython: unreachable virtual env dir " << ISSOFAPYTHON_VIRTUALENV_DIR << std::endl;
        // Trying in the parent dir
        std::string prefix = sofa::helper::system::SetDirectory::GetRelativeFromProcess("../");
        // Remove the trailing path separator, as we want the next call to GetFileName()
        // to return the dir name (otherwise it would return an empty string)
        if (prefix.back() == '/' || prefix.back() == '\\')
        {
            prefix.pop_back();
        }
        // Handle case where executables are in <builddir>/bin/<config>
        if (sofa::helper::system::SetDirectory::GetFileName(prefix.c_str()) == "bin")
        {
            prefix = sofa::helper::system::SetDirectory::GetParentDir(prefix.c_str());
        }
        potentialVenvDir = prefix + '/' + ISSOFAPYTHON_VIRTUALENV_DIRNAME;
        std::cout << "ISSofaPython: trying to find the the virtual env dir as " << potentialVenvDir << std::endl;
        if (sofa::helper::system::FileSystem::exists(potentialVenvDir) &&
            sofa::helper::system::FileSystem::isDirectory(potentialVenvDir))
        {
            virtualenvDir = potentialVenvDir;
        }
    }

    if (!virtualenvDir.empty())
    {
        std::cout << "ISSofaPython: setting the Python home to " << virtualenvDir << std::endl;
        Py_SetPythonHome(const_cast<char*>(virtualenvDir.c_str()));
    }
    else
    {
        std::cerr << "ERROR: ISSofaPython: could not find the virtual env dir" << std::endl;
    }
#endif // ISSOFAPYTHON_USE_VIRTUALENV
    std::cout << "ISSofaPython: initializing the Python interpreter" << std::endl;
    pybind11::initialize_interpreter();
}

const char* getModuleName()
{
    return "ISSofaPythonPlugin";
}

const char* getModuleVersion()
{
    return "0.0";
}
 
const char* getModuleLicense()
{
    return "Private";
}


const char* getModuleDescription()
{
    return "InSimo ISSofa Python binding library";
}

const char* getModuleComponentList()
{
    /// string containing the names of the classes provided by the plugin
    static std::string classes = sofa::core::ObjectFactory::getInstance()->listClassesFromTarget(sofa_tostring(SOFA_TARGET));
    return classes.c_str();
}


} // namespace python

} // namespace sofa

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

#include "initPlugin.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/config.h> // sofa_tostring
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/FileSystem.h>
#include "PythonSceneLoader.h"
#ifdef PYTHON_LIBRARY_SONAME
#include <sofa/helper/system/DynamicLibrary.h>
#include <dlfcn.h>
#endif

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

// Test the existence of a Python .py module file given its path.
// The input string should end with ".py", and the existence of the
// byte-compiled .pyc/.pyo versions are checked if the .py is not found.
static bool doesPythonModuleExist(const std::string& modulePath) {
    return (sofa::helper::system::FileSystem::exists(modulePath)
            || sofa::helper::system::FileSystem::exists(modulePath + 'c')
            || sofa::helper::system::FileSystem::exists(modulePath + 'o'));
}

void initExternalModule()
{
    if (Py_IsInitialized())
    {
        std::cout << "ISSofaPython: the Python interpreter is already initialized" << std::endl;
        return;
    }

    // On Linux when using a virtualenv, native libraries such as used by numpy fail to load unless the python library is manually loaded
#ifdef PYTHON_LIBRARY_SONAME
    std::cout << "ISSofaPython: manually load the python library shared object (required for ubuntu) : " << PYTHON_LIBRARY_SONAME << std::endl;
    sofa::helper::system::DynamicLibrary::DynamicLibrary::load(PYTHON_LIBRARY_SONAME, true);
#endif
    // Let's initialize the Python interpreter, possibly using the local env
    // (full python ditribution, or virtualenv) by setting the appropriate Python home dir.

#ifdef ISSOFAPYTHON_USE_LOCAL_ENV
    // On Windows, as soon as the Python env resides next to the process executable
    // (in the same dir or one level above), we could do nothing and the local env
    // would be used, but this is not the case on Linux. In all cases we force the
    // Python home here, as it also allows to be sure the local env is used even
    // when a PYTHONHOME env var is set on the system.

    // We could set some flags here, such as Py_IgnoreEnvironmentFlag and Py_NoUserSiteDirectory.
    // But for the moment we choose to be versatile and as much as possible have the
    // same behavior as the non wrapped python interpreter executable, especially
    // allowing to use env vars that can be useful for debugging purposes, or supporting
    // a virtualenv installed with the --system-site-packages option and relying on packages
    // installed in the user site dir.
    // We may set these flags in the future for production builds when embedding a standalone
    // python distribution, but our local sitecustomize.py module currently does the necessary
    // to ensure the local modules/packages are always prioritary versus other ones
    // potentially existing outside the local env.

    // using static here because we need a static storage for the call to Py_SetPythonHome
    static std::string pythonHome;

    std::string processeExeDir = sofa::helper::system::SetDirectory::GetRelativeFromProcess(".");
    // GetRelativeFromProcess(".") returns something like "<process dir>/.", let's clean this
    if (processeExeDir.back() == '.')
    {
        processeExeDir.pop_back();
        processeExeDir.pop_back();
    }

    // To properly detect where the Python home should be set, we use the os.py[c|o]
    // landmark module (this is common to use this module, and also done in the
    // Python source code itself).
    // If not found, we also try to find a "pythonXY.zip" file, as it can contain the
    // python libs, and is also used as a landmark since Python 3.
#ifdef WIN32
    #define PYLIB_LANDMARK_OS_PY "Lib/os.py"
    #define PYLIB_LANDMARK_SITE_PACKAGES "Lib/site-packages"
    #define PYLIB_LANDMARK_ZIP "python" sofa_tostring(PY_MAJOR_VERSION) sofa_tostring(PY_MINOR_VERSION) ".zip"
#else
    #define PYLIB_LANDMARK_OS_PY "lib/python" sofa_tostring(PY_MAJOR_VERSION) "." sofa_tostring(PY_MINOR_VERSION) "/os.py"
    #define PYLIB_LANDMARK_SITE_PACKAGES "lib/python" sofa_tostring(PY_MAJOR_VERSION) "." sofa_tostring(PY_MINOR_VERSION) "/site-packages"
    #define PYLIB_LANDMARK_ZIP "lib/python" sofa_tostring(PY_MAJOR_VERSION) sofa_tostring(PY_MINOR_VERSION) ".zip"
#endif

    if (doesPythonModuleExist(processeExeDir + "/" PYLIB_LANDMARK_OS_PY)
        || sofa::helper::system::FileSystem::exists(processeExeDir + "/" PYLIB_LANDMARK_SITE_PACKAGES)
        || sofa::helper::system::FileSystem::exists(processeExeDir + "/" PYLIB_LANDMARK_ZIP))
    {
        pythonHome = processeExeDir;
    }
    else if (doesPythonModuleExist(processeExeDir + "/../" PYLIB_LANDMARK_OS_PY)
        || sofa::helper::system::FileSystem::exists(processeExeDir + "/../" PYLIB_LANDMARK_SITE_PACKAGES)
        || sofa::helper::system::FileSystem::exists(processeExeDir + "/../" PYLIB_LANDMARK_ZIP))
    {
        pythonHome = processeExeDir + "/..";
    }

    if (!pythonHome.empty())
    {
#if PY_MAJOR_VERSION >= 3
        // Py_SetPythonHome does not work with Python 3.8.1, but Py_SetProgramName
        // works better (there are issues related to that on internet)
        std::string pythonProgramName = processeExeDir + "/python";
        std::cout << "ISSofaPython: setting the Python program name to " << pythonProgramName << std::endl;
        static std::wstring pythonProgramName_w = std::wstring(pythonProgramName.begin(), pythonProgramName.end());
        Py_SetProgramName(const_cast<wchar_t*>(pythonProgramName_w.c_str()));
#else
        std::cout << "ISSofaPython: setting the Python home to " << pythonHome << std::endl;
        Py_SetPythonHome(const_cast<char*>(pythonHome.c_str()));
#endif
    }
    else
    {
        std::cerr << "ERROR: ISSofaPython: could not find the local Python home" << std::endl;
    }
#endif // ISSOFAPYTHON_USE_LOCAL_ENV

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

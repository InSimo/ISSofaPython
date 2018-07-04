/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "initPlugin.h"
#include <sofa/core/ObjectFactory.h>
#include "PythonSceneLoader.h"

namespace sofa
{

namespace python
{


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
    static bool first = true;
    if (first)
    {
        using sofa::simulation::SceneLoaderFactory;
        using sofa::simulation::SceneLoader;
        // memory bleeding to death yo.
        SceneLoader* pythonSceneLoader = SceneLoaderFactory::getInstance()->addEntry(new PythonSceneLoader());
        first = false;
    }

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

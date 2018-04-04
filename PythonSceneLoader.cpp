/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "PythonSceneLoader.h"
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/simulation/tree/GNode.h>

namespace sofa
{

template< typename T>
using sptr = boost::intrusive_ptr<T>;

}

PYBIND11_DECLARE_HOLDER_TYPE(T, sofa::sptr<T>, true);


namespace sofa
{
namespace python
{

using sofa::simulation::SceneLoaderFactory;
using sofa::simulation::SceneLoader;

PythonSceneLoader::PythonSceneLoader()
{
    pybind11::initialize_interpreter();

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
    pybind11::finalize_interpreter();
    pybind11::initialize_interpreter();

    using sofa::helper::system::SetDirectory;
    using sofa::simulation::tree::GNode;
    using sofa::core::objectmodel::New;

    std::string fileDir = SetDirectory::GetParentDir(filename);
    std::string file    = SetDirectory::GetFileNameWithoutExtension(filename);

    GNode::SPtr gNode = New<GNode>("root");

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
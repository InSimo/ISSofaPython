/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_PYTHONSCENELOADER_H
#define ISSOFA_PYTHON_PYTHONSCENELOADER_H

#include <sofa/simulation/common/SceneLoaderFactory.h>
#include <pybind11/embed.h>

namespace sofa
{

namespace python
{

class PythonSceneLoader : public sofa::simulation::SceneLoader
{
public:

    PythonSceneLoader();

    ~PythonSceneLoader();

    bool canLoadFileExtension(const char *extension) override;

    std::string getFileTypeDesc() override;

    void getExtensionList(ExtensionList* list) override;

    sofa::simulation::Node::SPtr load(const char *filename) override;
};


}

}

#endif // ISSOFA_PYTHON_PYTHONSCENELOADER_H

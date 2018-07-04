/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/
#ifndef ISSOFA_PYTHON_INITPLUGIN_H
#define ISSOFA_PYTHON_INITPLUGIN_H


#include <sofa/SofaSimulation.h>


namespace sofa
{
namespace python
{

#ifdef SOFA_BUILD_ISSOFAPYTHONPLUGIN
#  define SOFA_TARGET ISSofaPython
#  define ISSOFAPYTHONPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define ISSOFAPYTHONPLUGIN_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

}

}



#endif // ISSOFA_PYTHON_INITPLUGIN_H

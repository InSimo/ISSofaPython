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
#ifndef ISSOFA_PYTHON_INITPLUGIN_H
#define ISSOFA_PYTHON_INITPLUGIN_H


#include <sofa/SofaSimulation.h>


namespace sofa
{
namespace python
{

#ifdef SOFA_BUILD_ISSOFAPYTHONPLUGIN
#  define SOFA_TARGET ISSofaPythonPlugin
#  define ISSOFAPYTHONPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define ISSOFAPYTHONPLUGIN_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

}

}



#endif // ISSOFA_PYTHON_INITPLUGIN_H

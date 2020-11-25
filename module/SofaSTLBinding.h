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

#ifndef ISSOFA_PYTHON_SOFASTLBINDING_H
#define ISSOFA_PYTHON_SOFASTLBINDING_H

#include <pybind11/stl.h>
#include <sofa/helper/vector.h>
#include <sofa/helper/set.h>
#include <sofa/helper/fixed_array.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/Quater.h>

namespace pybind11
{
namespace detail
{

template <typename Type, size_t Size> struct type_caster<sofa::helper::fixed_array<Type, Size> >
    : array_caster<sofa::helper::fixed_array<Type, Size>, Type, false, Size> { };

template <typename Type> struct type_caster<sofa::helper::vector<Type> >
    : list_caster<sofa::helper::vector<Type>, Type> { };

template <typename Key, typename Compare, typename Alloc> struct type_caster<sofa::helper::set<Key, Compare, Alloc> >
    : set_caster<sofa::helper::set<Key, Compare, Alloc>, Key> { };


template <typename Real, size_t Size> struct type_caster<sofa::defaulttype::Vec<Size, Real> >
    : array_caster<sofa::defaulttype::Vec<Size, Real>, Real, false, Size> { };

template <typename Real> struct type_caster<sofa::helper::Quater<Real> >
    : array_caster<sofa::helper::Quater<Real>, Real, false, 4> { };

}

}

#endif // ISSOFA_PYTHON_SOFASTLBINDING_H

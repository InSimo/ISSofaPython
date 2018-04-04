/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#ifndef ISSOFA_PYTHON_SOFASTLBINDING_H
#define ISSOFA_PYTHON_SOFASTLBINDING_H

#include <pybind11/stl.h>
#include <sofa/helper/vector.h>
#include <sofa/helper/set.h>
#include <sofa/helper/fixed_array.h>

namespace pybind11
{
namespace detail
{

template <typename Type, size_t Size> struct type_caster<sofa::helper::fixed_array<Type, Size> >
    : array_caster<std::array<Type, Size>, Type, false, Size> { };

template <typename Type, typename Alloc> struct type_caster<sofa::helper::vector<Type, Alloc> >
    : list_caster<std::vector<Type, Alloc>, Type> { };

template <typename Key, typename Compare, typename Alloc> struct type_caster<sofa::helper::set<Key, Compare, Alloc> >
    : set_caster<std::set<Key, Compare, Alloc>, Key> { };

}

}

#endif // ISSOFA_PYTHON_SOFASTLBINDING_H

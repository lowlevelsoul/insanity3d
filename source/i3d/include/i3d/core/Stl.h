//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_STL_H__
#define __I3D_STL_H__

#include <vector>
#include <map>
#include <string>

namespace i3d { namespace stl {
    
    template <typename T, typename A = StlAllocator<T> >
    struct Vector {
        typedef typename std::vector<T, A> type;
        typedef typename std::vector<T, A>::iterator iterator;
        typedef typename std::vector<T, A>::const_iterator const_iterator;
    };

    template <typename K, typename V, typename P = std::less<K>, typename A = StlAllocator<std::pair<const K, V> > >
    struct Map {
       typedef typename std::map<K, V, P, A> type;
       typedef typename std::map<K, V, P, A>::iterator iterator;
       typedef typename std::map<K, V, P, A>::const_iterator const_iterator;
    };

    template<typename T, typename TYPE_TRAITS, typename ALLOC>
    struct BasicString {
        typedef typename std::basic_string<T, TYPE_TRAITS, ALLOC> type;
        typedef typename std::basic_string<T, TYPE_TRAITS, ALLOC>::iterator iterator;
        typedef typename std::basic_string<T, TYPE_TRAITS, ALLOC>::const_iterator const_iterator;
        
        typedef typename type::size_type size_type;
        static constexpr size_type npos = -1;
    };

    typedef BasicString<char, std::char_traits<char>, StlAllocator<char> > String;

    typedef BasicString<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t> > WString;

#if 0
    typedef std::basic_string<char, std::char_traits<char>, StlAllocator<char> > String;

    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t> > WString;
#endif
}}

#endif

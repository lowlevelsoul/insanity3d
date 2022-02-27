//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_STLALLOCATOR_H__
#define __I3D_STLALLOCATOR_H__

namespace i3d { namespace stl {
    
    namespace StlAllocatorUtil {
        inline  void destruct(char*) {}
        inline  void destruct(wchar_t*) {}

        template <typename T>
        inline  void destruct(T* t) { t->~T(); }
    }

    template<typename T>
    class StlAllocator
    {
    public:
        typedef size_t          size_type;
        typedef int32_t         difference_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef T               value_type;
        
        template <class U> struct rebind
        {
            typedef StlAllocator<U> other;
        };
        
        StlAllocator() throw()
        {
            
        }
     
        StlAllocator(const StlAllocator&) throw()
        {
            
        }
        
        template <class U> StlAllocator(const StlAllocator<U>&) throw()
        {
            
        }
        
        ~StlAllocator() throw()
        {
            
        }

        pointer address(reference x) const
        {
            return &x;
        }
        
        const_pointer address(const_reference x) const
        {
            return &x;
        }

        pointer allocate(size_type size,
                        const T* hint = 0)
        {
            size_t actual_size = size * sizeof(T);
            pointer pMem = (pointer) malloc(actual_size);

            return pMem;
        }
        
        
        void deallocate(pointer p, size_type n)
        {
            free(p);
        }
        
        size_type max_size() const throw()
        {
            return 0xffffffff;
        }


        void construct(pointer p, const T& val)
        {
            new(static_cast<void*>(p)) T(val);
        }

#if 0
        void construct(pointer p)
        {
            new(static_cast<void*>(p)) T();
        }
#endif
        
        void destroy(pointer p)
        {
            StlAllocatorUtil::destruct<T>(p);
        }
    };


    template <class T1, class T2>
    inline bool operator==(const StlAllocator<T1>&, const StlAllocator<T2>&)
                    throw()
    {
        return true;
    }

    template <class T1, class T2>
    inline bool operator!=(const StlAllocator<T1>&, const StlAllocator<T2>&)
                    throw()
                    {
                    return false;
                    }
}}

#endif

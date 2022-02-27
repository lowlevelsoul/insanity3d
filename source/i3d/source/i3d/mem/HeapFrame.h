//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_HEAPFRAME_H__
#define __I3D_HEAPFRAME_H__

#include "i3d/core/Types.h"
#include <atomic>

namespace i3d {

    class HeapFrame {
    protected:
        HeapFrame( uintptr_t memory, size_t memorySize );
        
        ~HeapFrame();
        
    public:
        static HeapFrame * Create( uintptr_t memory, size_t memorySize );
        
        static void Destroy( HeapFrame * _self );
        
        template<typename __type__>
        __type__ * Alloc() {
            return (__type__ *) Alloc( sizeof( __type__) );
        }
        
        template<typename __type__>
        __type__ * Alloc( size_t count ) {
            size_t allocSize = count * sizeof(__type__);
            return (__type__ *) Alloc( allocSize );
        }
        
        void * Alloc( size_t size );
        
        void Reset();
        
    protected:
        uintptr_t                   m_memoryStart;
        uintptr_t                   m_memoryEnd;
        std::atomic<uintptr_t>      m_memoryPtr;
    };
}

#endif

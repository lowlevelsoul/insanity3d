//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/core/Sys.h"
#include "i3d/mem/HeapFrame.h"

namespace i3d {

    //======================================================================================================================
    HeapFrame * HeapFrame::Create( uintptr_t memory, size_t memorySize ) {
        
        size_t instSize = sizeof(HeapFrame) + 16;
        size_t newMemorySize = memorySize - instSize;
        uintptr_t newMemoryBase = i3d::Sys::AlignValue<uintptr_t>( memory + instSize, 16 );
        
        HeapFrame * inst = new( (void*) memory ) HeapFrame( newMemoryBase, newMemorySize );
        return inst;
    }


    //======================================================================================================================
    void HeapFrame::Destroy( HeapFrame * _self ) {
        if ( _self != nullptr ) {
            _self->~HeapFrame();
        }
    }

    //======================================================================================================================
    HeapFrame::HeapFrame( uintptr_t memory, size_t memorySize ) {
        m_memoryStart = memory;
        m_memoryEnd = memory + memorySize;
        m_memoryPtr.store(memory);
    }

    //======================================================================================================================
    HeapFrame::~HeapFrame() {
        
    }

    //=======================================================================================================================
    void * HeapFrame::Alloc( size_t size ) {
        uintptr_t oldPtr = 0;
        bool exchangeOk = false;
        
        do {
            // Get current pointer
            oldPtr = m_memoryPtr.load();
        
            // Calculate new pointer. If the new pointer is past the end of the memory
            // that we manage, then we're put of space. So bail with failure.
            uintptr_t newPtr = oldPtr + size;
            if  ( newPtr > m_memoryEnd ) {
                return nullptr;
            }
            
            // Attempt to update the internal pointer. If we set the new pointer succesfully,
            // then we can just bail. If not, another thread has nuked the pointer and
            // allocated memory during our own alloc, so loop again.
            exchangeOk = m_memoryPtr.compare_exchange_strong( oldPtr, newPtr );
            
        } while( exchangeOk == false );
            
        return (void *) oldPtr;
    }

    //=======================================================================================================================
    void HeapFrame::Reset() {
        m_memoryPtr.store( m_memoryStart );
    }
}

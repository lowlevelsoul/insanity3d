//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxBuffer_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    BufferMetal::BufferMetal() {
        m_size = 0;
    }
    
    //======================================================================================================================
    BufferMetal::~BufferMetal() {
        
    }
    
    //======================================================================================================================
    void BufferMetal::Create( size_t size, uint64_t flags ) {
        
        uint64_t  bufferFlags = 0;
        bufferFlags |= ((flags & BUFFER_GPU) != 0) ? MTLResourceStorageModePrivate : 0;
        
        m_buffer = [GetDevice() newBufferWithLength: size
                                            options: bufferFlags];
        
        m_size = m_size;
    }
    
    //======================================================================================================================
    void BufferMetal::Destroy() {
        m_buffer = nil;
        m_size = 0;
    }
    
    //======================================================================================================================
    void * BufferMetal::Map( uint64_t flags ) {
        return m_buffer.contents;
    }
    
    //======================================================================================================================
    void BufferMetal::Unmap() {
        
    }
    
    //======================================================================================================================
    void BufferMetal::Write( uintptr_t startOffs, size_t count, const void * src ) {
        XE_ASSERT( startOffs + count > m_size );
        
        uint8_t * mem  = ( uint8_t * ) Map( BUFFER_MAP_WRITEONLY);
        memcpy( mem + startOffs, src, count );
        Unmap();
    }
    
    //======================================================================================================================
    void BufferMetal::Read( uintptr_t startOffs, size_t count, void * src ) {
        XE_ASSERT( startOffs + count > m_size );
        
        uint8_t * mem  = ( uint8_t * ) Map( BUFFER_MAP_READONLY);
        memcpy( src, mem + startOffs, count );
        Unmap();
    }
}}


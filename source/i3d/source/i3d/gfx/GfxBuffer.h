//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXBUFFER_H__
#define __I3D_GFXBUFFER_H__

#include "i3d/core/Types.h"

namespace i3d { namespace gfx {
    
    class Buffer {
    public:
        virtual ~Buffer() {}
        
        virtual void Create( size_t size, uint64_t flags ) = 0;
        
        virtual void Destroy() = 0;
        
        virtual void * Map( uint64_t flags = 0) = 0;
        
        virtual void Unmap() = 0;
        
        virtual void Write( uintptr_t startOffs, size_t count, const void * src ) = 0;
        
        virtual void Read( uintptr_t startOffs, size_t count, void * src ) = 0;
        
        size_t GetSize() const { return m_size; }
        
    protected:
        size_t      m_size;         ///< Size of the buffer in bytes
        
    };
}}


#endif

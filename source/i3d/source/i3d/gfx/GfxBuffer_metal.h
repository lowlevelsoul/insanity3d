//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXBUFFER_METAL_H__
#define __I3D_GFXBUFFER_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxBuffer.h"

namespace i3d { namespace gfx {
    
    class BufferMetal : public Buffer {
    public:
        BufferMetal();
        
        virtual ~BufferMetal();
        
        virtual void Create( size_t size, uint64_t flags ) override;
        
        virtual void Destroy() override;
        
        virtual void * Map( uint64_t flags = 0) override;
        
        virtual void Unmap() override;
        
        virtual void Write( uintptr_t startOffs, size_t count, const void * src ) override;
        
        virtual void Read( uintptr_t startOffs, size_t count, void * src ) override;
        
    public:
        id<MTLBuffer>       m_buffer;        
    };
}}


#endif

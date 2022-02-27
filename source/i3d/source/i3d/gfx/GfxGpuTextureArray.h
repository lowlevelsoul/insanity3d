//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXGPUTEXTUREARRAY_H__
#define __GFXGPUTEXTUREARRAY_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

namespace i3d { namespace gfx {
    
    class Buffer;
    class Texture;
    
    class GpuTextureArray {
    public:
        
        struct Params {
            const char *        m_dummyFunctionName;
            uint32_t            m_bufferSlot;
            
            Params() {
                m_dummyFunctionName = nullptr;
                m_bufferSlot = 0;
            }
        };
        
        virtual ~GpuTextureArray() {}
        
        virtual void Create( const Params & params ) = 0;
        
        virtual size_t GetBufferSize() = 0;
        
        virtual void EncodeToBuffer( Buffer * buffer, Texture ** textures, size_t textureCount ) = 0;
        
        virtual size_t GetCapacity() const = 0;
    };
}}

#endif

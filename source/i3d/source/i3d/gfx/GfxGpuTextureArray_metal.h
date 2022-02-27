//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXGPUTEXTUREARRAY_METAL_H__
#define __I3D_GFXGPUTEXTUREARRAY_METAL_H__

#include <Metal/Metal.h>
#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"
#include "i3d/gfx/GfxGpuTextureArray.h"

namespace i3d { namespace gfx {
    
    class Buffer;
    class Texture;
    
    class GpuTextureArrayMetal : public GpuTextureArray {
    public:
        static constexpr size_t CAPACITY = 5000000;
        
        GpuTextureArrayMetal();
        
        virtual ~GpuTextureArrayMetal();
        
        virtual void Create( const Params & params ) override;
        
        virtual size_t GetBufferSize() override;
        
        virtual void EncodeToBuffer( Buffer * buffer, Texture ** textures, size_t textureCount ) override;
        
        virtual size_t GetCapacity() const override { return CAPACITY; }
        
    public:        
        id<MTLTexture>              m_textures[ CAPACITY ];
        id<MTLFunction>             m_fakeFunction;
        id<MTLArgumentEncoder>      m_encoder;
    };
}}

#endif

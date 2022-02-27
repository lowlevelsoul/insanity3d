//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXTEXTURE_METAL_H__
#define __I3D_GFXTEXTURE_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTexture.h"

#include <metal/Metal.h>

namespace i3d { namespace gfx {
    
    class TextureMetal : public Texture {
    public:
        
        TextureMetal();
        
        virtual ~TextureMetal();
        
        virtual void Create( FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) override;
        
        virtual void Write( const void * srcBuffer, uint32_t mip ) override;
        
    public:
        uint64_t                m_timestamp;
        uint64_t                m_poolIndex;
        id<MTLTexture>          m_texture;
        uint32_t                m_heapTexture : 1;
        uint32_t                m_heapTexturePending : 1;
        MTLPixelFormat          m_formatMetal;
        uint32_t                m_blockSizeX;
        uint32_t                m_blockSizeY;
        uint32_t                m_pitch;
        uint32_t                m_blockCountX;
        uint32_t                m_blockCountY;
        uint32_t                m_blockSize;
    };
}}

#endif

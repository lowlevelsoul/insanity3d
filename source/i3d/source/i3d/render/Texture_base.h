//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXTUREBASE_H__
#define __I3D_TEXTUREBASE_H__

#include "i3d/render/Texture.h"
#include "i3d/gfx/GfxTexture.h"
#include "i3d/res/Resource.h"

namespace i3d {
    class TextureBase : public Texture {
    public:
        DECLARE_RESOURCE( TextureBase )
        
        TextureBase();
        
        virtual ~TextureBase();
        
        virtual void Create( FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) override;
        
        virtual void Write( const void * srcBuffer, uint32_t mip ) override;
        
        virtual void Load( File * file, const char * path ) override;
        
    public:;
        uint32_t            m_poolIndex;
        uint64_t            m_timeStamp;
        gfx::Texture *      m_texture;
    };
}

#endif 

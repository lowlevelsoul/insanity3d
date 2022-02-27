//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXTUREARRAY_H__
#define __I3D_TEXTUREARRAY_H__

#include "i3d/core/Types.h"
#include "i3d/render/Texture_base.h"

namespace i3d {
    
    class Material;
    
    namespace gfx {
        class Texture;
    };
    
    class TextureArray {
    public:
        TextureArray();
        
        ~TextureArray();
        
        void Initialise( size_t capacity );
        
        void BeginFrame();
        
        void AddTextures( Material * mat );
        
        void AddTexture( Texture * tex );
        
        gfx::Texture ** GetTextures() { return m_textures; }
        
        size_t GetTextureCount() { return m_count; }
    
    public:
        uint64_t        m_timeStamp;
        gfx::Texture ** m_textures;
        size_t          m_capacity;
        size_t          m_count;
    };
    
    //======================================================================================================================
    inline void TextureArray::AddTexture( Texture * tex ) {
        
        TextureBase * texBase = (TextureBase *) tex;
        
        if ( texBase->m_timeStamp != m_timeStamp ) {
            texBase->m_timeStamp = m_timeStamp;
            texBase->m_poolIndex = (uint32_t) m_count;
            
            m_textures[ m_count ] = texBase->m_texture;
            ++m_count;
        }
    }
}

#endif


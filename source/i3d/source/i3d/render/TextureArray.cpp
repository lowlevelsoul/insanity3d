//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/Texture.h"
#include "i3d/render/Texture_base.h"
#include "i3d/render/Render_local.h"
#include "i3d/render/Material.h"
#include "i3d/render/TextureArray.h"

#define MATERIAL_TEXTURE_SAFE(tex, def) ((tex) != nullptr) ? (TextureBase *) (tex) : (TextureBase *) renderLocal->m_defaultTextures[def]

namespace i3d {
    
    //======================================================================================================================
    TextureArray::TextureArray() {
        m_capacity = 0;
        m_count = 0;
        m_timeStamp = 0;
    }
    
    //======================================================================================================================
    TextureArray::~TextureArray() {
        SAFE_DELETEARRAY( m_textures );
    }

    //======================================================================================================================
    void TextureArray::Initialise( size_t capacity ) {
        m_capacity = capacity;
        m_count = 0;
        m_timeStamp = 0;
        
        m_textures = new gfx::Texture*[ capacity ];
    }
    
    //======================================================================================================================
    void TextureArray::BeginFrame() {
        m_count = 0;
        ++m_timeStamp;
    }
    
    //======================================================================================================================
    void TextureArray::AddTextures( Material * mat ) {
    
        
        TextureBase * albedo = MATERIAL_TEXTURE_SAFE( mat->m_albedo, Render::DEFAULT_TEXTURE_RED );
        TextureBase * amr = MATERIAL_TEXTURE_SAFE( mat->m_amr, Render::DEFAULT_TEXTURE_GREY );
        TextureBase * glow = MATERIAL_TEXTURE_SAFE( mat->m_glow, Render::DEFAULT_TEXTURE_BLACK );
        
        AddTexture( albedo );
        AddTexture( amr );
        AddTexture( glow );
        
        mat->m_texturePoolId[ Material::STAGE_ALBEDO ] = albedo->m_poolIndex;
        mat->m_texturePoolId[ Material::STAGE_AMR ] = amr->m_poolIndex;
        mat->m_texturePoolId[ Material::STAGE_GLOW ] = glow->m_poolIndex;
        mat->m_texturePoolId[ 3 ] = 0xbabeface;
    }
}



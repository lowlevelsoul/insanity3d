//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/FileSystem.h"
#include "i3d/core/Log.h"
#include "i3d/core/Sys.h"
#include "i3d/render/Texture.h"

#include "i3d/render/Texture_base.h"
#include "i3d/render/TextureImageLoader.h"
#include "i3d/render/TexStream.h"
#include "i3d/gfx/Gfx.h"

namespace i3d {
    
    DEFINE_RESOURCE( TextureBase, "tex;"  )
    
    static const Texture::FORMAT TEX_FORMAT_TABLE[] = {
        Texture::FORMAT::RGB_U8,            // FORMAT_RGB_U8
        Texture::FORMAT::RGBA_U8,           // FORMAT_RGBA_U8
        Texture::FORMAT::RGB_BC1,           // FORMAT_RGB_BC1
        Texture::FORMAT::NIL,               // FORMAT_RGBA_BC1
        Texture::FORMAT::RGBA_BC2,          // FORMAT_RGBA_BC2
        Texture::FORMAT::RGBA_BC3,          // FORMAT_RGBA_BC3
        Texture::FORMAT::RGB_ETC2,          // FORMAT_RGB_ETC2
        Texture::FORMAT::RGBA_ETC2,         // FORMAT_RGB_ETC2
        Texture::FORMAT::NIL,
        Texture::FORMAT::NIL,
        Texture::FORMAT::NIL,
        Texture::FORMAT::NIL,
        Texture::FORMAT::NIL,
        Texture::FORMAT::NIL
    };
    
    //======================================================================================================================
    TextureBase::TextureBase() {
        m_texture = nullptr;
        m_poolIndex = 0;
        m_timeStamp = 0;
        
    }
    
    //======================================================================================================================
    TextureBase::~TextureBase() {
        SAFE_DELETE( m_texture )
    }
    
    //======================================================================================================================
    void TextureBase::Create( FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) {
        m_texture = gfx::CreateTexture( ( gfx::Texture::FORMAT ) format, width, height, mipCount);
    }
    
    //======================================================================================================================
    void TextureBase::Write( const void * srcBuffer, uint32_t mip ) {
        m_texture->Write( srcBuffer, mip );
    }
    
    //======================================================================================================================
    void TextureBase::Load( File * file, const char * path ) {
        
        size_t length = file->Length();
        uint8_t * buffer = (uint8_t*) new uint8_t[ length ]; //mem->Allocate( length, Memory::TAG::TEXTURE );
        XE_ASSERT( buffer != nullptr );
        
        size_t amtRead = file->Read( buffer, sizeof( uint8_t ), length );
        XE_ASSERT( amtRead == length );
        
        i3d::TexStream * stream = (i3d::TexStream *) buffer;
        XE_ASSERT( stream->m_version == i3d::TexStream::VERSION );
        
        Texture::FORMAT texFmt = TEX_FORMAT_TABLE[ stream->m_format ];
        XE_ASSERT( texFmt != Texture::FORMAT::NIL );
        Create( texFmt, stream->m_width, stream->m_height, stream->m_mipCount );
        
        for( uint32_t lvl = 0; lvl < stream->m_mipCount + 1; ++lvl ) {
            void * img = stream->GetImage( lvl );
            Write( img, lvl );
        }
        
        //gfx::MoveTextureToResourceHeap( m_texture, true );
        
        //mem->Free( buffer );
        SAFE_DELETEARRAY( buffer );
    }
}

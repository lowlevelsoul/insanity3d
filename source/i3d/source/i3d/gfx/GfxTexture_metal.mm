//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/Sys.h"
#include "i3d/gfx/GfxTexture_metal.h"
#include "i3d/gfx/Gfx_metal.h"


#if TARGET_OS_OSX == 1
#   define FORMAT_MACOS_ONLY(VAL) VAL
#else
#   define FORMAT_MACOS_ONLY(VAL) MTLPixelFormatInvalid
#endif

namespace i3d { namespace gfx {

    static const size_t PIXEL_SIZE_TABLE[] = {
        0,          // FORMAT_NONE = 0,
        
        3,          // FORMAT_RGB_S8,
        3,          // FORMAT_RGB_U8,
        4,          // FORMAT_RGBA_S8,
        4,          // FORMAT_RGBA_U8,
    };

    const MTLPixelFormat FORMAT_TABLE[] = {
        MTLPixelFormatInvalid,                          // FORMAT_NONE = 0,

        MTLPixelFormatInvalid,                          // FORMAT_RGB_S8
        MTLPixelFormatInvalid,                          // FORMAT_RGB_U8

        MTLPixelFormatRGBA8Snorm,                       // FORMAT_RGBA_S8
        MTLPixelFormatRGBA8Unorm,                       // FORMAT_RGBA_U8,
        
        FORMAT_MACOS_ONLY(MTLPixelFormatBC1_RGBA),      // FORMAT_RGBA_BC1,
        FORMAT_MACOS_ONLY(MTLPixelFormatBC2_RGBA),      // FORMAT_RGBA_BC2,
        FORMAT_MACOS_ONLY(MTLPixelFormatBC3_RGBA),      // FORMAT_RGBA_BC3
        
        MTLPixelFormatETC2_RGB8,                        // FORMAT_RGB_ETC2
        MTLPixelFormatInvalid,                          // FORMAT_RGBA_ETC2
    };
    
    //======================================================================================================================
    static size_t _textureGetPixelSize( Texture::FORMAT fmt ) {
        
        if ( fmt >= Texture::FORMAT::RGB_S8 && fmt <= Texture::FORMAT::RGBA_U8 ) {
            return PIXEL_SIZE_TABLE[ (uint32_t) fmt];
        }
        else if ( fmt >= Texture::FORMAT::RGB_BC1 && fmt <= Texture::FORMAT::RGBA_BC2 ) {
            return 8;
        }
        else if ( fmt == Texture::FORMAT::RGBA_BC3 ) {
            return 16;
        }
        else if ( fmt >= Texture::FORMAT::RGB_ETC2 && fmt <= Texture::FORMAT::RGBA_ETC2 ) {
            return 8;
        }
              
        return 0;
    }

    //======================================================================================================================
    static size_t _textureGetByteSize( Texture::FORMAT fmt, uint32_t width, uint32_t height ) {
      size_t pixelSize = _textureGetPixelSize( fmt );
      
      if ( fmt >= Texture::FORMAT::RGB_BC1 && fmt <= Texture::FORMAT::RGBA_BC3 ) {
          size_t size = ( ( width * height ) / 16 ) * pixelSize;
          return size;
      }
      else if ( fmt >= Texture::FORMAT::RGB_ETC2 && fmt <= Texture::FORMAT::RGB_ETC2 ) {
          size_t size = ( ( width * height ) / 16 ) * pixelSize;
          return size;
      }
      
      size_t size = ( width * height ) * pixelSize;
      return size;
    }

    //======================================================================================================================
    static bool _textureIsBlockCompressed( Texture::FORMAT format) {
        return (format >= Texture::FORMAT::RGB_BC1 && format <= Texture::FORMAT::RGBA_ETC2);
    }
    
    //======================================================================================================================
    TextureMetal::TextureMetal() {
        m_timestamp = 0;
        m_poolIndex = 0;
    }
    
    //======================================================================================================================
    TextureMetal::~TextureMetal() {
        
    }
    
    //======================================================================================================================
    void TextureMetal::Create( Texture::FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) {
        // What's the metal format?
        MTLPixelFormat mtlFormat = FORMAT_TABLE[ (uint32_t) format];
        XE_ASSERT( mtlFormat != MTLPixelFormatInvalid );
        
        uint32_t usageFlags = 0;
    #if 0
        if (flags & GFX_TEX_RENDER_TARGET) {
            usageFlags |= MTLTextureUsageRenderTarget;
        }
        
        if (flags & GFX_TEX_SHADER_READ) {
            usageFlags |= MTLTextureUsageShaderRead;
        }
    #endif
        
        m_format    = format;
        m_width     = width;
        m_height    = height;
        m_mipCount  = mipCount;
        m_pitch     = width * (uint32_t) _textureGetPixelSize( format );
        
        // Create the metal texture object
        MTLTextureDescriptor* desc = [ MTLTextureDescriptor new ];
        
        desc.textureType            = MTLTextureType2D;
        desc.width                  = width;
        desc.height                 = height;
        desc.pixelFormat            = mtlFormat;
        desc.usage                  = usageFlags;
        desc.mipmapLevelCount       = mipCount + 1;
        
        m_texture = [ GetDevice() newTextureWithDescriptor:desc ];
        
        bool isBc = _textureIsBlockCompressed( format );
        if (isBc == true) {
            m_blockSizeX = 4;
            m_blockSizeY = 4;
            m_blockCountX = m_width / m_blockSizeX;
            m_blockCountY = m_height / m_blockSizeY;
            m_blockSize = (uint32_t) _textureGetPixelSize( format );
        }
        else {
            m_blockSizeX = 0;
            m_blockSizeY = 0;
            m_blockCountX = 0;
            m_blockCountY = 0;
            m_blockSize = 0;
        }
    }
    
    //======================================================================================================================
    void TextureMetal::Write( const void * srcBuffer, uint32_t mip ) {
        
        MTLRegion region;
        region.origin.x = 0;
        region.origin.y = 0;
        region.origin.z = 0;

        region.size.width = (m_width >> mip);
        region.size.height = (m_height >> mip);
        region.size.depth = 1;
       
        NSUInteger bytesPerRow = (m_pitch >> mip);
        
        if (_textureIsBlockCompressed( m_format )) {
            bytesPerRow = (region.size.width / m_blockSizeX) * m_blockSize;
        }
        
        [m_texture replaceRegion: region
                     mipmapLevel: mip
                       withBytes: srcBuffer
                     bytesPerRow: bytesPerRow];
    }
}}

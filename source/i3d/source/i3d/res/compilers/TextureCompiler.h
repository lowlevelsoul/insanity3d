//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_TEXTURECOMPILER_H__
#define __XENGINE_TEXTURECOMPILER_H__

#include "xe/res/ResourceCompiler.h"
#include "xe/util/Image.h"
#include "xe/render/TexStream.h"

namespace xe {
    
    class Resource;
    class Image;

    class TextureCompiler : public ResourceCompiler  {
    public:
        RTTI_CLASS_DECLARE( TextureCompiler, ResourceCompiler )
        
        struct BlockCompressFormat {
            const char * formatString;
            Image::FORMAT format;
        };
        
        static const BlockCompressFormat BLOCK_FORMATS[];
        
        TextureCompiler();
        
        virtual ~TextureCompiler();
        
        virtual void GetDependencies( Resource * resource,
                                      xe::Array<xe::String> & inputs ) override;
        
        virtual void Compile( Resource * resource ) override;
        
        
    protected:
        
        Image * LoadImage();
        
        Image * Resize( Image* src, uint32_t newWidth, uint32_t newHeight );
        
        Image::FORMAT StringToBlockFormat( const char* str );
        
        Image * BlockCompress( Image* src, Image::FORMAT blockFormat );
        
        Image * BlockCompressBC( Image* src, Image::FORMAT blockFormat );
        
        Image * BlockCompressETC( Image* src, Image::FORMAT blockFormat );
        
        uint32_t GetSquishFlags( Image::FORMAT blockFormat );
        
        bool WriteImages( xe::File * str, xe::Array<xe::Image::ref_ptr_t> & images );
        
        bool WriteHeader( xe::File * str, xe::TexStream& header );
        
        xe::TexStream::FORMAT GetStreamFormat( Image::FORMAT imgFmt );
        
    public:
        xe::String                  m_input;            ///< The path to the input texture
        xe::String                  m_block;            ///< The block format string
        uint32_t                    m_mipCount;         ///< How many mip levels the texture should have
        uint32_t                    m_maxSize;          ///< Maximum texture size (width or height)
        xe::Array<Image::ref_ptr_t> m_images;           ///< The array of images
        xe::Image::ref_ptr_t        m_baseImage;         ///< The source image that was loaded
    };
}

#endif

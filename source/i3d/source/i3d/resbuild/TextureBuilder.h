//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXTURECOMPILER_H__
#define __I3D_TEXTURECOMPILER_H__

#include "i3d/res/ResourceBuilder.h"
#include "i3d/util/Image.h"
#include "i3d/render/TexStream.h"

namespace i3d {
    
    class Resource;
    class Image;

    class TextureBuilder : public ResourceBuilder  {
    public:
        RTTI_CLASS_DECLARE( TextureBuilder, ResourceBuilder )
        
        struct BlockCompressFormat {
            const char * formatString;
            Image::FORMAT format;
        };
        
        static const BlockCompressFormat BLOCK_FORMATS[];
        
        TextureBuilder();
        
        virtual ~TextureBuilder();
        
        virtual void GetDependencies( Resource * resource,
                                      stl::Vector<stl::String::type>::type & inputs ) override;
        
        virtual void Compile( Resource * resource ) override;
        
        
    protected:
        
        Image * LoadImage();
        
        Image * Resize( Image* src, uint32_t newWidth, uint32_t newHeight );
        
        Image::FORMAT StringToBlockFormat( const char* str );
        
        Image * BlockCompress( Image* src, Image::FORMAT blockFormat );
        
        Image * BlockCompressBC( Image* src, Image::FORMAT blockFormat );
        
        Image * BlockCompressETC( Image* src, Image::FORMAT blockFormat );
        
        uint32_t GetSquishFlags( Image::FORMAT blockFormat );
        
        bool WriteImages( File * str, stl::Vector<Image::ref_ptr_t>::type & images );
        
        bool WriteHeader( File * str, TexStream& header );
        
        TexStream::FORMAT GetStreamFormat( Image::FORMAT imgFmt );
        
    public:
        stl::String::type                       m_input;            ///< The path to the input texture
        stl::String::type                       m_block;            ///< The block format string
        uint32_t                                m_mipCount;         ///< How many mip levels the texture should have
        uint32_t                                m_maxSize;          ///< Maximum texture size (width or height)
        stl::Vector<Image::ref_ptr_t>::type     m_images;           ///< The array of images
        Image::ref_ptr_t                        m_baseImage;         ///< The source image that was loaded
    };
}

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/resbuild/TextureBuilder.h"
#include "i3d/util/Image.h"

#include "avir.h"
#include "squish.h"
#include "Etc.h"
#include "EtcImage.h"

namespace i3d {
    
    RTTI_CLASS_BEGIN( TextureBuilder )
        RTTI_PROP( STRING,  "input",        m_input )
        RTTI_PROP( STRING,  "block",        m_block )
        RTTI_PROP( INT,     "mip_count",    m_mipCount )
        RTTI_PROP( INT,     "max_size",     m_maxSize )
    RTTI_CLASS_END( TextureBuilder )
    
    const TextureBuilder::BlockCompressFormat TextureBuilder::BLOCK_FORMATS[] = {
        {"bc1",                 Image::FORMAT_RGB_BC1},
        {"bc2",                 Image::FORMAT_RGBA_BC2},
        {"bc3",                 Image::FORMAT_RGBA_BC3},
        {"etc2",                Image::FORMAT_RGB_ETC2},
        {"etc2a",               Image::FORMAT_RGBA_ETC2},
        { nullptr }
    };

    //======================================================================================================================
    TextureBuilder::TextureBuilder() {
        m_mipCount = 0;
        m_maxSize = 0;
    }

    //======================================================================================================================
    TextureBuilder::~TextureBuilder() {
        
    }

    //======================================================================================================================
    void TextureBuilder::GetDependencies( Resource * resource,
                                         stl::Vector<stl::String::type>::type & inputs ) {
        if ( m_input.empty() == false ) {
            inputs.push_back( m_input );
        }
    }
    
    //======================================================================================================================
    void TextureBuilder::Compile( Resource * resource ) {
        // Load the base image and add to the array of images
        Image * baseImg = LoadImage();
        m_images.push_back( baseImg );
        
        // If required, generate mip maps
        if ( m_mipCount > 0 ) {
            XE_LOG("Generating %u mip maps\n", m_mipCount );
            
            uint32_t mipWidth = baseImg->GetWidth();
            uint32_t mipHeight = baseImg->GetHeight();
            
            for ( uint32_t mip = 0; mip < m_mipCount; ++mip ) {
                mipWidth /= 2;
                mipHeight /= 2;
                
                if ( mipWidth < 32 || mipHeight < 32 ) {
                    break;
                }
                
                XE_LOG("    Generating mip %u (%u x %u)\n", mip, mipWidth, mipHeight );
                Image * mipImage = Resize( m_baseImage, mipWidth, mipHeight );
                m_images.push_back( mipImage );
            }
        }
        
        // Block compress the images, replacing the old uncompressed images.
        if ( m_block.empty() == false ) {
            
            Image::FORMAT blockFormat = StringToBlockFormat( m_block.c_str() );
            XE_ERROR( m_baseImage->IsBlockCompressed( blockFormat ) == false, "Format %s is not a valid block format\n", m_block.c_str() );
            
            XE_LOG("    Compressing images to block format '%s'\n", m_block.c_str() );
                        
            uint32_t currImage = 0;
            for ( auto & src : m_images ) {
                XE_LOG("        Image %u (%u x %u)\n", currImage, src->GetWidth(), src->GetHeight());
                Image * blockImage = BlockCompress( src, blockFormat );
                src = blockImage;
                ++currImage;
            }
        }
        
        // Write texture stream from images
        XE_LOG( "Writing texture images\n" );
        
        bool folderOk = CreateFolderAtPath( resource->GetPath() );
        XE_ERROR( folderOk == false, " Could not create folder at %s\n", resource->GetPath() );
        
        i3d::ScopedFile file( resource->GetPath(), "wb" );
        XE_ERROR( file.IsValid() == false, "Error opening resource file %s for writing\n", resource->GetPath());
        
        WriteImages( file, m_images );
    }
    
    //======================================================================================================================
    Image * TextureBuilder::LoadImage() {
        
        Image * img = nullptr;
        bool loadedImage = false;
        
        if ( m_baseImage == nullptr ) {
        
            XE_LOG("Loading base image from %s\n", m_input.c_str() );
            
            img = new Image;
            bool loadOk = img->Load( m_input.c_str() );
            XE_ERROR(loadOk == false, "Uncaught error loading texture image %s\n", m_input.c_str() );
            
            loadedImage = true;
        }
        
        // There's no maximum image size, so just return the source image
        if ( m_maxSize == 0) {
            
            if (loadedImage == true ) {
                m_baseImage = img;
                return img;
            }
            
            return m_baseImage;
        }
        else {
            m_baseImage = img;
        }
        
        // If we have a maximum size, check to see if the image violates that contraint. If it
        // does, we'll resize and return the resized image to the caller
        if (img->GetWidth() > m_maxSize || img->GetHeight() > m_maxSize ) {
            uint32_t newWidth, newHeight;
            if ( img->GetWidth() > m_maxSize ) {
                uint32_t factor = img->GetHeight() / img->GetWidth();
                newWidth = m_maxSize;
                newHeight = m_maxSize * factor;
            }
            else {
                uint32_t factor = img->GetWidth() / img->GetHeight();
                newWidth = m_maxSize * factor;
                newHeight = m_maxSize;
            }
            
            XE_LOG("    Resizing base image to %u x %u\n", newWidth, newHeight);
            img = Resize( img, newWidth, newHeight );
        }
        
        return img;
    }
    
    //===============================================================================================================================
    Image* TextureBuilder::Resize( Image* src, uint32_t newWidth, uint32_t newHeight) {
        Image * rgbaImage;
        
        if (src->GetWidth() != newWidth || src->GetHeight() != newHeight) {
            // If the width and/or height are different to the input image, then we need to resize
            
            size_t pixelComponents = src->GetPixelComponents();
            
            rgbaImage = new Image;
            rgbaImage->Initialise(newWidth, newHeight, src->m_format);
            
            avir :: CImageResizer<> imageResizer( 8 );
            imageResizer.resizeImage( &src->m_bytes[0], src->GetWidth(), src->GetHeight(), 0,
                                      &rgbaImage->m_bytes[0], newWidth, newHeight, (uint32_t) pixelComponents, 0 );
                    
        }
        else {
            // Output image size is the same as the input, so just set the uncompressed src image to be
            // the input
            rgbaImage = src;
        }
        
        return rgbaImage;
    }
    
    //===============================================================================================================================
    Image::FORMAT TextureBuilder::StringToBlockFormat(const char* str) {
        
        for(const BlockCompressFormat * fmt = BLOCK_FORMATS; fmt->formatString != nullptr; ++fmt) {
            if ( strcasecmp(str, fmt->formatString) == 0 ) {
                return fmt->format;
            }
        }
            
        return Image::FORMAT_COUNT;
    }
    
    //===============================================================================================================================
    Image* TextureBuilder::BlockCompress( Image* src, Image::FORMAT blockFormat) {
        assert( src->IsBlockCompressed( blockFormat )) ;
    
        Image * dst = nullptr;

        if ( ( blockFormat >= Image::FORMAT_RGB_BC1 ) && ( blockFormat <= Image::FORMAT_RGBA_BC3 ) ) {
            dst = BlockCompressBC( src, blockFormat );
        }
        else {
            dst = BlockCompressETC( src, blockFormat );
        }
        
        return dst;
    }
    
    //===============================================================================================================================
    uint32_t TextureBuilder::GetSquishFlags( Image::FORMAT blockFormat ) {
        uint32_t sqFlags = 0;
        
        switch(blockFormat) {
            case Image::FORMAT_RGB_BC1:
            case Image::FORMAT_RGBA_BC1:
                sqFlags |= squish::kDxt1;
                break;
                
            case Image::FORMAT_RGBA_BC2:
                sqFlags |= squish::kDxt3;
                break;
                
            case Image::FORMAT_RGBA_BC3:
                sqFlags |= squish::kDxt5;
                break;
                
            default:
                break;
        }
        
        return sqFlags;
    }
    
    
    //===============================================================================================================================
    Image * TextureBuilder::BlockCompressBC( Image * src, Image::FORMAT blockFormat) {
        
        bool validSrc =  ( ( ( src->GetFormat() == Image::FORMAT_RGB_U8 ) || ( src->GetFormat() == Image::FORMAT_RGBA_U8 ) ));
        XE_ERROR(validSrc == false, "Cannot block compress image from this format\n" );
        
        Image * dst = new Image;
        dst->Initialise(src->m_width, src->m_height, blockFormat);
        
        uint32_t sqFlags = GetSquishFlags(blockFormat);
    
        size_t dstImgSize = squish::GetStorageRequirements(src->m_width, src->m_height, sqFlags);
        XE_ASSERT(dstImgSize == dst->m_size);

        
        if ( blockFormat > Image::FORMAT_RGB_BC1 && src->GetFormat() != Image::FORMAT_RGBA_U8 ) {
            Image * rgbaCopy = new Image;
            rgbaCopy->Initialise( src->GetWidth(), src->GetHeight(), Image::FORMAT_RGBA_U8 );
            
            rgbaCopy->Blit( *src, 0, 0 );
            squish::CompressImage( &rgbaCopy->m_bytes[0], src->GetWidth(), src->GetHeight(), &dst->m_bytes[0], sqFlags);
            
            SAFE_DELETE( rgbaCopy );
        }
        else {
            squish::CompressImage( &src->m_bytes[0], src->GetWidth(), src->GetHeight(), &dst->m_bytes[0], sqFlags);
        }
        
        return dst;
    }
    
    //===============================================================================================================================
    Image * TextureBuilder::BlockCompressETC( Image * src, Image::FORMAT blockFormat) {
        
        Image * dst = nullptr;
        
        stl::Vector<Etc::ColorFloatRGBA>::type floatPixels;
        floatPixels.resize( src->m_width * src->m_height );
        
        if ( src->m_format == Image::FORMAT_RGB_U8 ) {
            
            uint8_t * srcRgba = &src->m_bytes[0];
            for( Etc::ColorFloatRGBA & fp : floatPixels ) {
                fp = Etc::ColorFloatRGBA::ConvertFromRGBA8( srcRgba[0], srcRgba[1], srcRgba[2], 255);
                srcRgba += src->m_pixelSize;
            }
        }
        else if ( src->m_format == Image::FORMAT_RGBA_U8 ) {
            uint8_t * srcRgba = &src->m_bytes[0];
            for( Etc::ColorFloatRGBA & fp : floatPixels ) {
                fp = Etc::ColorFloatRGBA::ConvertFromRGBA8( srcRgba[0], srcRgba[1], srcRgba[2], srcRgba[3] );
                srcRgba += src->m_pixelSize;
            }
        }
        else {
            XE_ERROR( false, "Bad etc input image format\n" );
        }
        
        uint8_t * encodedBits;
        uint32_t encodedBitsNumBytes;
        uint32_t encodedBitsWidth;
        uint32_t encodedBitsHeight;
        int32_t encodedTime = 0;
        
        Etc::Image::Format etcFormat = Etc::Image::Format::RGB8;        // ( blockFormat == Image::FORMAT_RGB_ETC2) ? Etc::Image::Format::RGB8 : Etc::Image::Format::RGBA8;
        Etc::Encode((float*) &floatPixels[0], src->m_width, src->m_height, etcFormat, Etc::RGBX, 1, 4, 5, &encodedBits, &encodedBitsNumBytes, &encodedBitsWidth, &encodedBitsHeight, &encodedTime );
        
        dst = new Image;
        dst->Initialise(src->m_width, src->m_height, blockFormat);
        
        XE_ERROR( encodedBitsNumBytes != dst->m_size, "Encoding size / expected image size mismatch \n" );
        memcpy( &dst->m_bytes[0], encodedBits, encodedBitsNumBytes );
        
        return dst;
    }
    
    //===============================================================================================================================
    bool TextureBuilder::WriteImages( i3d::File * str, stl::Vector<i3d::Image::ref_ptr_t>::type & images ) {
        i3d::TexStream header;
        memset(&header, 0, sizeof(header));
        
        // Fill out the info that we currently know about the header, and write it to the file
        header.m_version    = i3d::TexStream::VERSION;
        header.m_width      = images[0]->GetWidth();
        header.m_height     = images[0]->GetHeight();
        header.m_mipCount   = (uint32_t) images.size() - 1;
        header.m_format     = GetStreamFormat(images[0]->m_format);
        
        uintptr_t headerPos = str->Pos();
        WriteHeader(str, header);
            
        // Write the image data
        for(uint32_t i=0; i < images.size(); ++i) {
            
            str->WriteAlignPadding( 64 );
            header.m_offsImages[i] = (uint32_t) (str->Pos() - headerPos);
            
            str->Write<uint8_t>(&images[i]->m_bytes[0], images[i]->m_size );
        }
        
        // Go back and re-write the header
        uintptr_t endPos = str->Pos();
        str->Seek( headerPos );
        WriteHeader( str, header );
        str->Seek( endPos );
        
        return true;
    }

    //===============================================================================================================================
    bool TextureBuilder::WriteHeader( i3d::File * str, i3d::TexStream& header ) {
        str->Write( &header.m_version );
        str->Write( &header.m_flags );
        str->Write( &header.m_format );
        str->Write( &header.m_width );
        str->Write( &header.m_height );
        str->Write( &header.m_mipCount );
        str->Write( header.m_offsImages, i3d::TexStream::MAX_MIP_COUNT );
        
        return true;
    }
    
    //===============================================================================================================================
    i3d::TexStream::FORMAT TextureBuilder::GetStreamFormat( Image::FORMAT imgFmt ) {
        switch(imgFmt) {
            case Image::FORMAT_RGB_U8:
                return i3d::TexStream::FORMAT_RGB_U8;
                
            case Image::FORMAT_RGBA_U8:
                return i3d::TexStream::FORMAT_RGBA_U8;
            
            case Image::FORMAT_RGB_BC1:
                return i3d::TexStream::FORMAT_RGB_BC1;
                
            case Image::FORMAT_RGBA_BC1:
                return i3d::TexStream::FORMAT_RGBA_BC1;
                
            case Image::FORMAT_RGBA_BC2:
                return i3d::TexStream::FORMAT_RGBA_BC2;
                
            case Image::FORMAT_RGBA_BC3:
                return i3d::TexStream::FORMAT_RGBA_BC3;
                
            case Image::FORMAT_RGB_ETC2:
                return i3d::TexStream::FORMAT_RGB_ETC2;
                
            case Image::FORMAT_RGBA_ETC2:
                return i3d::TexStream::FORMAT_RGBA_ETC2;
                
            default:
                break;
        }
        
        return i3d::TexStream::FORMAT_NONE;
    }
}

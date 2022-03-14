//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "texbuilder/TexBuilderApp.h"
#include "texbuilder/TexBuilder.h"
#include "squish.h"
#include "avir.h"
#include "Etc.h"
#include "EtcImage.h"

const TexBuilder::BlockCompressFormat TexBuilder::BLOCK_FORMATS[] = {
    {"bc1",                 ToolImage::FORMAT_RGB_BC1},
    {"bc2",                 ToolImage::FORMAT_RGBA_BC2},
    {"bc3",                 ToolImage::FORMAT_RGBA_BC3},
    {"etc2",                ToolImage::FORMAT_RGB_ETC2},
    {"etc2a",               ToolImage::FORMAT_RGBA_ETC2},
    { nullptr }
};

//===============================================================================================================================
TexBuilder::TexBuilder() {
    m_resize    = false;
    m_newWidth = 0;
    m_newHeight = 0;
    
    m_genMips = false;
    m_mipCount = 0;
    
    m_convertToBlock = false;
    m_outputFormat = ToolImage::FORMAT_RGB_BC1;
}

//===============================================================================================================================
TexBuilder::~TexBuilder() {
    
}

//===============================================================================================================================
bool TexBuilder::WriteOutputImage( std::string& errorMsg ) {
    
    r3d::ScopedFile texFile( m_outPath.c_str(), "wb");
    if (texFile.IsValid() == false ) {
        errorMsg = sys->Vformat("Error opening %s", m_outPath.c_str());
        return false;
    }
    
    LOG("Writing to %s\n", m_outPath.c_str());
    
    bool writeOk = false;
    if (m_convertToBlock == true) {
        LOG("    Writing block compressed images\n");
        writeOk = WriteImages(texFile, m_dstImagesCompressed);
    }
    else {
        LOG("    Writing uncompressed images\n");
        writeOk = WriteImages(texFile, m_dstImages);
    }
    
    if (writeOk == false) {
        errorMsg = sys->Vformat("Error writing to %s", m_outPath.c_str());
        return false;
    }
    
    return true;
}

//===============================================================================================================================
ToolImage::FORMAT TexBuilder::EnumStringToFormat(const char* str) {
    
    for(const TexBuilder::BlockCompressFormat * fmt = BLOCK_FORMATS; fmt->formatString != nullptr; ++fmt) {
        if ( strcasecmp(str, fmt->formatString) == 0 ) {
            return fmt->format;
        }
    }
        
    return ToolImage::FORMAT_COUNT;
}

//===============================================================================================================================
ToolImage* TexBuilder::BlockCompress(ToolImage* src, ToolImage::FORMAT blockFormat) {
    assert(src->IsBlockCompressed(blockFormat));
    
    
    ToolImage* dst = nullptr;

    
    if ( ( blockFormat >= ToolImage::FORMAT_RGB_BC1 ) && ( blockFormat <= ToolImage::FORMAT_RGBA_BC3 ) ) {
        
        dst = new ToolImage;
        dst->Initialise(src->m_width, src->m_height, blockFormat);
    
        uint32_t sqFlags = GetSquishFlags(blockFormat);
    
        size_t dstImgSize = squish::GetStorageRequirements(src->m_width, src->m_height, sqFlags);
        assert(dstImgSize == dst->m_size);
    
        squish::CompressImage(&src->m_bytes[0], src->GetWidth(), src->GetHeight(), &dst->m_bytes[0], sqFlags);
    }
    else {
        
        std::vector<Etc::ColorFloatRGBA> floatPixels;
        floatPixels.resize( src->m_width * src->m_height );
        
        
        if ( src->m_format == ToolImage::FORMAT_RGB_U8 ) {
            
            uint8_t * srcRgba = &src->m_bytes[0];
            for( Etc::ColorFloatRGBA & fp : floatPixels ) {
                fp = Etc::ColorFloatRGBA::ConvertFromRGBA8( srcRgba[0], srcRgba[1], srcRgba[2], 255);
                srcRgba += src->m_pixelSize;
            }
        }
        else if ( src->m_format == ToolImage::FORMAT_RGBA_U8 ) {
            uint8_t * srcRgba = &src->m_bytes[0];
            for( Etc::ColorFloatRGBA & fp : floatPixels ) {
                fp = Etc::ColorFloatRGBA::ConvertFromRGBA8( srcRgba[0], srcRgba[1], srcRgba[2], srcRgba[3] );
                srcRgba += src->m_pixelSize;
            }
        }
        else {
            ERROR( false, "Bad etc input image format\n" );
        }
        
        uint8_t * encodedBits;
        uint32_t encodedBitsNumBytes;
        uint32_t encodedBitsWidth;
        uint32_t encodedBitsHeight;
        int32_t encodedTime = 0;
        
        Etc::Image::Format etcFormat = Etc::Image::Format::RGB8;        // ( blockFormat == ToolImage::FORMAT_RGB_ETC2) ? Etc::Image::Format::RGB8 : Etc::Image::Format::RGBA8;
        Etc::Encode((float*) &floatPixels[0], src->m_width, src->m_height, etcFormat, Etc::RGBX, 1, 4, 5, &encodedBits, &encodedBitsNumBytes, &encodedBitsWidth, &encodedBitsHeight, &encodedTime );
        
        dst = new ToolImage;
        dst->Initialise(src->m_width, src->m_height, blockFormat);
        
        ERROR( encodedBitsNumBytes != dst->m_size, "Encoding size / expected image size mismatch \n" );
        memcpy( &dst->m_bytes[0], encodedBits, encodedBitsNumBytes );
    }
    
    return dst;
}

//===============================================================================================================================
ToolImage* TexBuilder::AstcCompress(ToolImage* src, ToolImage::FORMAT blockFormat) {
    return nullptr;
}

//===============================================================================================================================
uint32_t TexBuilder::GetSquishFlags(ToolImage::FORMAT blockFormat) {
    uint32_t sqFlags = 0;
    
    switch(blockFormat) {
        case ToolImage::FORMAT_RGB_BC1:
        case ToolImage::FORMAT_RGBA_BC1:
            sqFlags |= squish::kDxt1;
            break;
            
        case ToolImage::FORMAT_RGBA_BC2:
            sqFlags |= squish::kDxt3;
            break;
            
        case ToolImage::FORMAT_RGBA_BC3:
            sqFlags |= squish::kDxt5;
            break;
            
        default:
            break;
    }
    
    return sqFlags;
}

//===============================================================================================================================
bool TexBuilder::LoadSourceImage( std::string& errorMsg ) {
    LOG("Loading input image %s\n", m_srcPath.c_str());
    
    m_srcImage = new ToolImage;
    bool loadOk = m_srcImage->Load(m_srcPath.c_str(), true);
    if (loadOk == false) {
        errorMsg = sys->Vformat("Error loading %s", m_srcPath.c_str());
        return false;
    }
    
    return true;
}

//===============================================================================================================================
bool TexBuilder::Process(  std::string& errorMsg ) {
    
    if (m_srcImage == nullptr) {
        bool loadOk = LoadSourceImage(errorMsg);
        if (loadOk == false) {
            return false;
        }
    }
    
    ToolImage::Ptr baseImage;
    
    // If we want a different sized base image, then we need to resize it. If not
    // we just use the source image.
    if (m_resize == true) {
        LOG("Resizing base image to %u x %u \n", m_newWidth, m_newHeight);
        baseImage = Resize(m_srcImage, m_newWidth, m_newHeight);
    }
    else {
        LOG("Using original base image \n");
        baseImage = m_srcImage;
    }
    
    m_dstImages.push_back(baseImage);
    
    if (m_genMips == true) {
        
        LOG("Generating mips\n");
    
        // Generate rgba mip images
        uint32_t smallestMipSize = 64;
        
        uint32_t mipWidth = baseImage->GetWidth();
        uint32_t mipHeight = baseImage->GetHeight();
        
        for(int32_t i=0; i < m_mipCount; ++i) {
            mipWidth /= 2;
            mipHeight /= 2;
            
            if (mipWidth < smallestMipSize || mipHeight < smallestMipSize) {
                break;
            }
            
            LOG("    Level %i (%u x %u)\n", i+1, mipWidth, mipHeight);
            
            ToolImage* mip = Resize(baseImage, mipWidth, mipHeight);
            m_dstImages.push_back(mip);
        }
    }
    
    if (m_convertToBlock == true) {
        LOG("Converting to block format\n");
        
        int32_t index = 0;
        for(auto s : m_dstImages) {
            LOG("    Compressing image %i (%u x %u)...", index, s->GetWidth(), s->GetHeight());
            ++index;
            
            ToolImage* blockImg = BlockCompress(s, m_outputFormat);
            m_dstImagesCompressed.push_back(blockImg);
            
            LOG("Done.\n");
        }
    }
    
    printf("Done\n");
    
    return true;
}

//===============================================================================================================================
ToolImage* TexBuilder::ResizeAndCompress(ToolImage* src, uint32_t newWidth, uint32_t newHeight, ToolImage::FORMAT blockFormat) {
    ToolImage::Ptr rgbaImage = Resize(src, newWidth, newHeight);
    ToolImage* compressed = BlockCompress(rgbaImage, blockFormat);
    
    return compressed;
}

//===============================================================================================================================
ToolImage* TexBuilder::Resize(ToolImage* src, uint32_t newWidth, uint32_t newHeight) {
    ToolImage * rgbaImage;
    
    if (src->GetWidth() != newWidth || src->GetHeight() != newHeight) {
        // If the width and/or height are different to the input image, then we need to resize
        
        size_t pixelComponents = src->GetPixelComponents();
        
        rgbaImage = new ToolImage;
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
bool TexBuilder::WriteImages( r3d::File * str, std::vector<ToolImage::Ptr>& images) {
    r3d::TexStream header;
    memset(&header, 0, sizeof(header));
    
    // Fill out the info that we currently know about the header, and write it to the file
    header.m_version    = r3d::TexStream::VERSION;
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
bool TexBuilder::WriteHeader( r3d::File * str, r3d::TexStream& header ) {
    str->Write( &header.m_version );
    str->Write( &header.m_flags );
    str->Write( &header.m_format );
    str->Write( &header.m_width );
    str->Write( &header.m_height );
    str->Write( &header.m_mipCount );
    str->Write( header.m_offsImages, r3d::TexStream::MAX_MIP_COUNT );
    
    return true;
}

//===============================================================================================================================
r3d::TexStream::FORMAT TexBuilder::GetStreamFormat(ToolImage::FORMAT imgFmt) {
    switch(imgFmt) {
        case ToolImage::FORMAT_RGB_U8:
            return r3d::TexStream::FORMAT_RGB_U8;
            
        case ToolImage::FORMAT_RGBA_U8:
            return r3d::TexStream::FORMAT_RGBA_U8;
        
        case ToolImage::FORMAT_RGB_BC1:
            return r3d::TexStream::FORMAT_RGB_BC1;
            
        case ToolImage::FORMAT_RGBA_BC1:
            return r3d::TexStream::FORMAT_RGBA_BC1;
            
        case ToolImage::FORMAT_RGBA_BC2:
            return r3d::TexStream::FORMAT_RGBA_BC2;
            
        case ToolImage::FORMAT_RGBA_BC3:
            return r3d::TexStream::FORMAT_RGBA_BC3;
            
        case ToolImage::FORMAT_RGB_ETC2:
            return r3d::TexStream::FORMAT_RGB_ETC2;
            
        case ToolImage::FORMAT_RGBA_ETC2:
            return r3d::TexStream::FORMAT_RGBA_ETC2;
            
        default:
            break;
    }
    
    return r3d::TexStream::FORMAT_NONE;
}


//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TEXBUILDER_H__
#define __TEXBUILDER_H__

#include "toolimage/ToolImage.h"
#include "engine/render/TexStream.h"
#include "texbuilder/AmrBuilder.h"

class TexBuilder {
public:
    struct BlockCompressFormat {
        const char * formatString;
        ToolImage::FORMAT format;
    };
    
    static const BlockCompressFormat BLOCK_FORMATS[];
    
    std::string         m_outPath;          // Path to the tex image we write to
    std::string         m_srcPath;          // Source image

    ToolImage::Ptr                        m_srcImage;               // Source image
    std::vector<ToolImage::Ptr>           m_dstImages;              // Array of images that represent the base image and the mip levels
    std::vector<ToolImage::Ptr>           m_dstImagesCompressed;    // Array of compressed images that represent the base image and the mip levels
    
    bool                    m_resize;
    uint32_t                m_newWidth;
    uint32_t                m_newHeight;

    bool                    m_genMips;
    uint32_t                m_mipCount;

    bool                    m_convertToBlock;
    ToolImage::FORMAT       m_outputFormat;
    
    TexBuilder();
    
    virtual ~TexBuilder();
    
    bool Process( std::string& errorMsg );
    
    bool WriteOutputImage( std::string& errorMsg );
    
    ToolImage::FORMAT EnumStringToFormat( const char * str );
    
    ToolImage* BlockCompress( ToolImage * src, ToolImage::FORMAT blockFormat );
    
    ToolImage* AstcCompress( ToolImage * src, ToolImage::FORMAT blockFormat );
    
    uint32_t GetSquishFlags( ToolImage::FORMAT blockFormat );
    
    ToolImage* ResizeAndCompress( ToolImage * src, uint32_t newWidth, uint32_t newHeight, ToolImage::FORMAT blockFormat );
    
    ToolImage* Resize( ToolImage * src, uint32_t newWidth, uint32_t newHeight );
    
    bool LoadSourceImage( std::string& errorMsg );
    
protected:
    
    r3d::TexStream::FORMAT GetStreamFormat( ToolImage::FORMAT imgFmt );
    
    bool WriteImages( r3d::File * str, std::vector<ToolImage::Ptr> & images);
    
    bool WriteHeader( r3d::File * str, r3d::TexStream& header);
};

#endif

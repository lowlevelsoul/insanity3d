//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "toolimage/ToolImage.h"
#include "i3d/core/FileSystem.h"
    
static const size_t FORMAT_PIXEL_SIZE[] = {
    0,          // FORMAT_NONE=0,
    
    3,          // FORMAT_RGB_U8,
    4,          // FORMAT_RGB_S8,
    3,          // FORMAT_RGBA_U8,
    4,          // FORMAT_RGBA_S8,
    
    12,         // FORMAT_RGB_FLOAT,
    16,         // FORMAT_RGBA_FLOAT,
};

//===============================================================================================================================
ToolImage::ToolImage() {
    m_format = FORMAT_NONE;
    m_width = 0;
    m_height = 0;
    m_pixelSize = 0;
    m_pitch = 0;
    m_size = 0;
};

//===============================================================================================================================
ToolImage::~ToolImage() {
    Finalise();
}

//===============================================================================================================================
void ToolImage::GetBlockCompressInfo( size_t& blockSizeX, size_t& blockSizeY, size_t& blockSizeBytes, FORMAT format ) const {
    assert( IsBlockCompressed( format ) == true);
    
    switch( format ) {
            
        case FORMAT_RGB_BC1:            // Fall through
        case FORMAT_RGBA_BC1:
            blockSizeX = 4;
            blockSizeY = 4;
            blockSizeBytes = 8;
            break;
            
        case FORMAT_RGBA_BC2:           // Fall through
        case FORMAT_RGBA_BC3:
            blockSizeX = 4;
            blockSizeY = 4;
            blockSizeBytes = 16;
            break;
            
        case FORMAT_RGB_ETC2:           // Fall through
        case FORMAT_RGBA_ETC2:
            blockSizeX = 4;
            blockSizeY = 4;
            blockSizeBytes = 8;
            break;
            
        default:
            break;
    }
}

//===============================================================================================================================
void ToolImage::Initialise( uint32_t width, uint32_t height, FORMAT format ) {
    assert(format != FORMAT_NONE && format < FORMAT_COUNT);
    
    if ( !IsCompressed( format ) ) {
        // Normal linear RGB or RGBA image image - so just allocate
        // as normal.
        Initialise( width, height, (uint32_t) FORMAT_PIXEL_SIZE[format] );
        m_format = format;
    }
    else if ( IsBlockCompressed( format ) ) {
        // A block compression
        GetBlockCompressInfo( m_blockDimX, m_blockDimY, m_blockSize, format );
        
        m_blockCountX = width / m_blockDimX + (((width % m_blockDimX) != 0) ? 1 : 0);
        m_blockCountY = height / m_blockDimY + (((height % m_blockDimY) != 0) ? 1 : 0);
        m_blockCount = m_blockCountX * m_blockCountY;
        m_size = m_blockSize * m_blockCount;
        
        m_bytes.resize( m_size );
        
        m_width = (int32_t)(m_blockCountX * m_blockDimX);
        m_height = (int32_t)(m_blockCountY * m_blockDimY);
        
        m_format = format;
    }
}

//===============================================================================================================================
void ToolImage::Initialise( uint32_t width, uint32_t height, uint32_t pixelSize ) {
    assert( width != 0 );
    assert( height != 0 );
    assert( pixelSize != 0 );
    
    // Clean up incase the image is being reused.
    Finalise();
    
    m_format = FORMAT_NONE;
    m_width = width;
    m_height = height;
    m_pixelSize = pixelSize;
    m_pitch = width * pixelSize;
    m_size = m_pitch * height;
    
    m_bytes.resize( m_size );
    m_rows.resize( m_height );
    
    uintptr_t currOffs = 0;
    
    for( uint32_t y=0; y < m_height; ++y ) {
        m_rows[ y ] = &m_bytes[ currOffs ];
        currOffs += m_pitch;
    }
}

//===============================================================================================================================
void ToolImage::Finalise() {
    m_format = FORMAT_NONE;
    m_width = 0;
    m_height = 0;
    m_pixelSize = 0;
    m_pitch = 0;
    m_size = 0;
}

//===============================================================================================================================
bool ToolImage::Load( const char* path, bool forceRgba ) {
    i3d::stl::String::type ext;
    bool gotExt = fs->GetExtension( ext, path );
    if (gotExt == false) {
        return false;
    }

    bool loadRes = false;
    
    if ( strcmp(ext.c_str(), "png" ) == 0 ) {
        loadRes = LoadPng( path, forceRgba );
    }
#if 0
    else if ( ( strcmp(ext.c_str(), "jpg" ) == 0 ) || ( strcmp(ext.c_str(), "jpeg" ) == 0 ) ) {
        loadRes = LoadJpeg( path, forceRgba );
    }
#endif
    else {
        return false;
    }
    
    return loadRes;
}

//===============================================================================================================================
bool ToolImage::Save( const char* path ) {
    i3d::ScopedFile file( path, "wb" );
    if ( file.IsValid() == false ) {
        return false;
    }
    
    bool res = SavePng( file ) ;
    return res;
}

//===============================================================================================================================
void ToolImage::SubImage( ToolImage& dst, uint32_t x, uint32_t y, uint32_t width, uint32_t height ) {
    assert(x+width <= m_width);
    assert(y+height <= m_height);
    
    // If the destination image doesn't have the desired size or pixel size, initialise it
    // to make it ready to use
    bool initDst = (dst.m_width == width) || (dst.m_height != height) || (dst.m_pixelSize = m_pixelSize);
    if (initDst) {
        dst.Initialise(width, height, (uint32_t)m_pixelSize);
    }
    
    for(uint32_t yp = 0; yp < height; ++yp ) {
        for(uint32_t xp = 0; xp < width; ++xp ) {
            uint8_t* dstP = dst.GetPixelAddress(xp, yp);
            const uint8_t* srcP = GetPixelAddress(x+xp, x+yp);
            
            switch(m_pixelSize) {
                case 4:
                    *dstP = *srcP;
                    ++dstP;
                    ++srcP;
                case 3:
                    *dstP = *srcP;
                    ++dstP;
                    ++srcP;
                case 2:
                    *dstP = *srcP;
                    ++dstP;
                    ++srcP;
                case 1:
                    *dstP = *srcP;
                    ++dstP;
                    ++srcP;
                    break;
                default:
                    memcpy(dstP, srcP, m_pixelSize);
                    break;
            }
        }
    }
}

//===============================================================================================================================
void ToolImage::ExpandRoRgba( uint32_t h, const uint8_t* srcRgb ) {
    uint8_t* dst = m_rows[h];
    const uint8_t* src = srcRgb;
    
    for(uint32_t x=0;  x < m_width; ++x) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 255;
        
        src += 3;
        dst += 4;
    }
}

//===============================================================================================================================
bool ToolImage::CalcImageResize( uint32_t& newWidth, uint32_t& newHeight, uint32_t maxSize ) {
    if (this->IsLargerThanSize(maxSize) == false) {
        newWidth = m_width;
        newHeight = m_height;
        return false;
    }
    
    if (m_width <= m_height) {
        float factor = (float)m_height / (float)m_width;
        newWidth = maxSize;
        newHeight = (uint32_t)(((float)maxSize) * factor);
        return true;
    }
    else {
        float factor = (float)m_width / (float)m_height;
        newHeight = maxSize;
        newWidth = (uint32_t)(((float)maxSize) * factor);
        return true;
    }
    
    
}

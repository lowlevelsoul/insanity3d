//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TOOLIMAGE_H__
#define __TOOLIMAGE_H__

#include "i3d/core/Types.h"
#include "i3d/util/RefPointer.h"
#include "i3d/util/RefObject.h"

namespace i3d {

    class Image : public RefObject {
    public:
        typedef i3d::RefPointer<Image> ref_ptr_t;
        enum FORMAT {
            FORMAT_NONE=0,
            
            FORMAT_RGB_U8,
            FORMAT_RGBA_U8,
            
            FORMAT_RGB_S8,
            FORMAT_RGBA_S8,
            
            FORMAT_RGB_FLOAT,
            FORMAT_RGBA_FLOAT,
            
            FORMAT_RGB_BC1,
            FORMAT_RGBA_BC1,
            FORMAT_RGBA_BC2,
            FORMAT_RGBA_BC3,
            
            FORMAT_RGB_ETC2,
            FORMAT_RGBA_ETC2,
            
            FORMAT_COUNT,
            
            FORMAT_BLOCK_START = FORMAT_RGB_BC1,
            FORMAT_BLOCK_END = FORMAT_RGBA_ETC2,
            
            FORMAT_FORCE32 = 0xffffffff

        };
        
        struct Rgba {
            float   m_red;
            float   m_green;
            float   m_blue;
            float   m_alpha;
        };
        
        FORMAT                      m_format;
        uint32_t                    m_width;
        uint32_t                    m_height;
        size_t                      m_pixelSize;
        size_t                      m_pitch;
        size_t                      m_size;
        size_t                      m_blockDimX;                ///< Width of the compressed block in pixels
        size_t                      m_blockDimY;                ///< Height of the cimpression block in pixels
        size_t                      m_blockSize;                ///< Size of a singe block in bytes
        size_t                      m_blockCount;               ///< Total block count
        size_t                      m_blockCountX;              ///< Number of blocks across
        size_t                              m_blockCountY;              ///< Number of blocks down
        stl::Vector<uint8_t>::type          m_bytes;                    ///< Image bytes
        stl::Vector<uint8_t*>::type         m_rows;                     ///< Image row pointers
        
    public:
        Image();
        
        virtual ~Image();
        
        void Initialise(uint32_t width, uint32_t height, uint32_t pixelSize);
        
        void Initialise(uint32_t width, uint32_t height, FORMAT format);
        
        bool Load(const char* path, bool forceRgba = false );
        
        bool Save(const char* path);
        
        void Finalise();
        
        void SubImage(Image& dst, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        
        void Blit( Image & dst, uint32_t x, uint32_t y );
        
        void Blit( Image & dst, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t width, uint32_t height );
        
        uint8_t * GetPixelAddress(uint32_t x, uint32_t y) const;
        
        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        size_t GetPitch() const { return m_pitch; }
        size_t GetPixelSize() const { return m_pixelSize; }
        size_t GetPixelComponents() const;
        
        void GetRgba(uint32_t x, uint32_t y, float& r, float& g, float& b, float& a) const;
        
        void SetRgba(uint32_t x, uint32_t y, float r, float g, float b, float a = 1);
        
        float CalcIntensity(int32_t x, int32_t y) const;
        
        bool IsSameSizeAs(const Image& rhs) const;
        
        bool IsFormatRgb() const { return m_format == FORMAT_RGB_U8 || m_format == FORMAT_RGB_S8 || m_format == FORMAT_RGB_FLOAT; }
        
        bool IsCompressed() const { return m_format >= FORMAT_RGB_BC1 && m_format <= FORMAT_RGBA_ETC2; }
        
        bool IsCompressed(FORMAT format) const { return format >= FORMAT_RGB_BC1 && m_format <= FORMAT_RGBA_ETC2; }
        
        bool IsBlockCompressed(FORMAT format) const { return format >= FORMAT_BLOCK_START && format <= FORMAT_BLOCK_END; }
        
        bool IsBlockCompressed() const { return IsBlockCompressed(m_format); }
        
        bool IsLargerThanSize(uint32_t size) { return (size < m_width || m_size < m_height); }
        
        bool CalcImageResize(uint32_t& newWidth, uint32_t& newheight, uint32_t maxSize);
        
        FORMAT GetFormat() const { return m_format; }
        
        void ExpandToRGBA( Image * src );
        
        void GetPixelAt( Rgba & dst, uint32_t x, uint32_t y) const;
        
        bool IsRgb() const { return m_format == FORMAT_RGB_U8 || FORMAT_RGB_S8 == FORMAT_RGB_FLOAT; }
        
        bool IsRgba() const { return m_format == FORMAT_RGBA_U8 || FORMAT_RGBA_S8 == FORMAT_RGBA_FLOAT; }
        
    protected:
        void GetBlockCompressInfo(size_t& blockSizeX, size_t& blockSizeY, size_t& blockSizeBytes, FORMAT format) const ;
        
        bool LoadPng(const char* path, bool forceAlpha);
        
        //bool LoadJpeg(const char* path, bool forceAlpha);
        
        bool SavePng( i3d::File * file );
        
        void ExpandRoRgba(uint32_t h, const uint8_t* srcRgb);

        template<typename __dstType__, typename __srcType__> __dstType__ Convert(const __srcType__& rhs) const {}
        
        template<> float Convert<float, uint8_t>(const uint8_t& rhs) const { return ((float)rhs) / 255.0f; }
        template<> float Convert<float, int8_t>(const int8_t& rhs) const { return (((float)rhs) + 127.0f) / 255.0f; }
        template<> float Convert<float, float>(const float& rhs) const { return rhs; }

        template<> uint8_t Convert<uint8_t, float>(const float& rhs) const { return (uint8_t)(255.0f * rhs); }
        template<> int8_t Convert<int8_t, float>(const float& rhs) const { return (int8_t)(255.0f * (rhs - 0.5f)); }
    };

    //===============================================================================================================================
    inline uint8_t * Image::GetPixelAddress(uint32_t x, uint32_t y) const {
        assert(x < m_width);
        assert(y < m_height);
        
        uint8_t * row = m_rows[y];
        uint8_t * pixel = row + (m_pixelSize * x);
        
        return pixel;
    }

    //===============================================================================================================================
    inline void Image::GetRgba(uint32_t x, uint32_t y, float& r, float& g, float& b, float& a) const {
        const uint8_t* bytes = GetPixelAddress(x, y);
        
        switch(m_format) {
            case FORMAT_RGB_U8:
                r = Convert<float, uint8_t>(bytes[0]);
                g = Convert<float, uint8_t>(bytes[1]);
                b = Convert<float, uint8_t>(bytes[2]);
                a = 1;
                break;
                
            case FORMAT_RGBA_U8:
                r = Convert<float, uint8_t>(bytes[0]);
                g = Convert<float, uint8_t>(bytes[1]);
                b = Convert<float, uint8_t>(bytes[2]);
                a = Convert<float, uint8_t>(bytes[3]);
                break;
                
            case FORMAT_RGB_S8:
                r = Convert<float, int8_t>(((int8_t*)bytes)[0]);
                g = Convert<float, int8_t>(((int8_t*)bytes)[1]);
                b = Convert<float, int8_t>(((int8_t*)bytes)[2]);
                a = 1;
                break;
                
            case FORMAT_RGBA_S8:
                r = Convert<float, int8_t>(((int8_t*)bytes)[0]);
                g = Convert<float, int8_t>(((int8_t*)bytes)[1]);
                b = Convert<float, int8_t>(((int8_t*)bytes)[2]);
                a = Convert<float, int8_t>(((int8_t*)bytes)[3]);
                break;
        
            case FORMAT_RGB_FLOAT:
                r = Convert<float, float>(((float*)bytes)[0]);
                g = Convert<float, float>(((float*)bytes)[1]);
                b = Convert<float, float>(((float*)bytes)[2]);
                a = 1;
                break;
                
            case FORMAT_RGBA_FLOAT:
                r = Convert<float, float>(((float*)bytes)[0]);
                g = Convert<float, float>(((float*)bytes)[1]);
                b = Convert<float, float>(((float*)bytes)[2]);
                a = Convert<float, float>(((float*)bytes)[3]);
                break;
            default:
                break;
        }
    }
        
    //===============================================================================================================================
    inline void Image::SetRgba(uint32_t x, uint32_t y, float r, float g, float b, float a) {
        uint8_t* bytes = GetPixelAddress(x, y);
        
        switch(m_format) {
                
            case FORMAT_RGB_U8:
                bytes[0] = Convert<uint8_t, float>(r);
                bytes[1] = Convert<uint8_t, float>(g);
                bytes[2] = Convert<uint8_t, float>(b);
                break;
                
            case FORMAT_RGBA_U8:
                bytes[0] = Convert<uint8_t, float>(r);
                bytes[1] = Convert<uint8_t, float>(g);
                bytes[2] = Convert<uint8_t, float>(b);
                bytes[3] = Convert<uint8_t, float>(a);
                break;
                
            case FORMAT_RGB_S8:
                ((int8_t*)bytes)[0] = Convert<int8_t, float>(r);
                ((int8_t*)bytes)[1] = Convert<int8_t, float>(g);
                ((int8_t*)bytes)[2] = Convert<int8_t, float>(b);
                break;
                
            case FORMAT_RGBA_S8:
                ((int8_t*)bytes)[0] = Convert<int8_t, float>(r);
                ((int8_t*)bytes)[1] = Convert<int8_t, float>(g);
                ((int8_t*)bytes)[2] = Convert<int8_t, float>(b);
                ((int8_t*)bytes)[3] = Convert<int8_t, float>(a);
                break;
        
            case FORMAT_RGB_FLOAT:
                ((float*)bytes)[0] = Convert<float, float>(r);
                ((float*)bytes)[1] = Convert<float, float>(g);
                ((float*)bytes)[2] = Convert<float, float>(b);
                break;
                
            case FORMAT_RGBA_FLOAT:
                ((float*)bytes)[0] = Convert<float, float>(r);
                ((float*)bytes)[1] = Convert<float, float>(g);
                ((float*)bytes)[2] = Convert<float, float>(b);
                ((float*)bytes)[3] = Convert<float, float>(a);
                break;
                
            default:
                break;
        }
    }

    //===============================================================================================================================
    inline float Image::CalcIntensity(int32_t x, int32_t y) const {
        float r, g, b, a;
        GetRgba(x, y, r, g, b, a);
        
        float intensity = (r + g + b) / 3.0f;
        return intensity;
    }

    //===============================================================================================================================
    inline bool Image::IsSameSizeAs(const Image& rhs) const {
        return (rhs.m_width == m_width && rhs.m_height == m_height);
    }

    //===============================================================================================================================
    inline size_t Image::GetPixelComponents() const {
        
        switch(m_format) {
            case FORMAT_RGB_U8:
            case FORMAT_RGBA_U8:
            case FORMAT_RGB_S8:
            case FORMAT_RGBA_S8:
                return m_pixelSize;
                
            case FORMAT_RGB_FLOAT:
            case FORMAT_RGBA_FLOAT:
                return m_pixelSize / 4;
            default:
                break;
        }
        
        return 0;
    }
}

#endif

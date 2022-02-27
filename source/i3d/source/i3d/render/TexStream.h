//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXSTREAM_H__
#define __I3D_TEXSTREAM_H__

namespace i3d {

    class TexStream {
    public:
        static const uint32_t VERSION = 2;
        static const size_t MAX_MIP_COUNT = 32;
        
        enum FORMAT {
            FORMAT_RGB_U8 = 0,
            FORMAT_RGBA_U8,
            FORMAT_RGB_BC1,
            FORMAT_RGBA_BC1,
            FORMAT_RGBA_BC2,
            FORMAT_RGBA_BC3,
            
            FORMAT_RGB_ETC2,
            FORMAT_RGBA_ETC2,
            
            FORMAT_NONE = 0xffffffff
        };
        
        static const uint32_t F_IMAGE_BIN = 0x00000001;         ///< If set in the flags, the image data is contained in a separate texi file, and offsets describe the offset of each mip in that file
        
        uint32_t        m_version;
        uint32_t        m_flags;
        uint32_t        m_format;
        uint32_t        m_width;
        uint32_t        m_height;
        uint32_t        m_mipCount;
        uint32_t        m_offsImages[MAX_MIP_COUNT];           ///< Offset of the array containing the offsets of the texture image data
        
        template<typename __type__>
        __type__* GetPointer(uintptr_t offset) const {
            return (__type__*) (((uintptr_t)this) + offset);
        }
        
        uint8_t* GetImage(uint32_t index) {
            return GetPointer<uint8_t>(m_offsImages[index]);
        }
    };
}

#endif

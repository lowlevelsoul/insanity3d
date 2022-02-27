//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXTURE_H__
#define __I3D_TEXTURE_H__

#include "i3d/core/Types.h"
#include "i3d/res/Resource.h"

namespace i3d {
    class Texture : public Resource {
    public:
        enum class FORMAT {
            NIL = 0,
           
            RGB_S8,
            RGB_U8,

            RGBA_S8,
            RGBA_U8,

            RGB_BC1,
            RGBA_BC2,
            RGBA_BC3,

            RGB_ETC2,
            RGBA_ETC2,
           
            COUNT,
       };
        
        virtual ~Texture() {}
        
        virtual void Create( FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) = 0;
        
        virtual void Write( const void * srcBuffer, uint32_t mip ) = 0;
        
        FORMAT GetFormat() const { return m_format; }
        uint32_t GetWidth() const { return m_width; };
        uint32_t GetHeight() const { return m_height; }
        uint32_t GetMipCount() const { return m_mipCount; }
        
    protected:
        FORMAT          m_format;
        uint32_t        m_width;
        uint32_t        m_height;
        uint32_t        m_mipCount;
    };
}

#endif

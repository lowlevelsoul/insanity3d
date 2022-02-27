//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXVERTEXDESC_H__
#define __I3D_GFXVERTEXDESC_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

namespace i3d { namespace gfx {
    
    class VertexDesc {
    public:
        struct Element {
            TYPE            m_type;
            uint32_t        m_offset;
        };
        
        virtual ~VertexDesc() {}
        
        virtual void Create( const Element * elements ) = 0;
        
        virtual void Destroy() = 0;
        
        size_t GetStride() const { return m_stride; }
            
    protected:
        size_t      m_stride;         ///< Complete size of the vertex description        
    };
}}


#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXVERTEXDESC_DESC_H__
#define __I3D_GFXVERTEXDESC_DESC_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxVertexDesc.h"

namespace i3d { namespace gfx {
    
    class VertexDescLocal : public VertexDesc {
    public:
        struct ElementInfo {
            gfx::TYPE   m_type;
            uint32_t    m_offset;
            uint32_t    m_size;
        };
        
        VertexDescLocal();
        
        virtual ~VertexDescLocal();
        
        virtual void Create( const Element * elements ) override;
        
        virtual void Destroy() override;
        
        const ElementInfo * GetElements() const { return m_elements; }
            
    protected:
        static constexpr size_t MAX_ELEMENTS = 16;
        ElementInfo         m_elements[MAX_ELEMENTS+2];
    };
}}


#endif

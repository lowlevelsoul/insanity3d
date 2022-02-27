//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxVertexDesc_local.h"

namespace i3d { namespace gfx {
    
    static constexpr size_t SIZE_TABLE[] = {
        0,
        4, 8, 12, 16,
        4, 8, 12, 16,
        4, 8, 12, 16
    };

    //======================================================================================================================
    VertexDescLocal::VertexDescLocal() {
        m_stride = 0;
    }
    
    //======================================================================================================================
    VertexDescLocal::~VertexDescLocal() {
        
    }
    
    //======================================================================================================================
    void VertexDescLocal::Create( const Element * elements ) {
        
        uint32_t i;
        m_stride = 0;
        
        for ( i=0; elements[i].m_type != gfx::TYPE_NIL; ++i ) {
            XE_ASSERT( i < MAX_ELEMENTS );
            
            m_elements[i].m_type = elements[i].m_type;
            m_elements[i].m_offset = elements[i].m_offset;
            m_elements[i].m_size = (uint32_t) SIZE_TABLE[ elements[i].m_type ];
            
            m_stride +=SIZE_TABLE[ elements[i].m_type ];
        }
        
        m_elements[i].m_type = gfx::TYPE_NIL;
    }
    
    //======================================================================================================================
    void VertexDescLocal::Destroy() {
        
    }
}}

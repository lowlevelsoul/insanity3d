//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXTYPES_H__
#define __I3D_GFXTYPES_H__

#include "i3d/core/Types.h"

namespace i3d { namespace gfx {
    
    static constexpr uint64_t BUFFER_STATIC             = BIT(0);
    static constexpr uint64_t BUFFER_DYNAMIC            = BIT(1);
    static constexpr uint64_t BUFFER_GPU                = BIT(2);
    static constexpr uint64_t BUFFER_CPU                = BIT(3);
    
    static constexpr uint64_t BUFFER_MAP_READONLY       = BIT(0);
    static constexpr uint64_t BUFFER_MAP_WRITEONLY      = BIT(1);
    static constexpr uint64_t BUFFER_MAP_READWRITE      = BIT(2);
    
    static constexpr uint64_t RENDER_STAGE_VERTEX       = BIT(0);
    static constexpr uint64_t RENDER_STAGE_PIXEL        = BIT(1);
    
    enum TYPE {
        TYPE_NIL,
        
        TYPE_FLOAT,
        TYPE_FLOAT2,
        TYPE_FLOAT3,
        TYPE_FLOAT4,
        
        TYPE_INT,
        TYPE_INT2,
        TYPE_INT3,
        TYPE_INT4,
        
        TYPE_UINT,
        TYPE_UINT2,
        TYPE_UINT3,
        TYPE_UINT4
    };
    
    enum PRIM {
        PRIM_POINTS,
        PRIM_TRIANGLES,
        PRIM_TRISTRIPS,
        PRIM_LINES,
        PRIM_LINESTRIP
    };
    
    enum FUNC {
        FUNC_ALWAYS = 0,
        FUNC_NEVER,
        FUNC_EQUAL,
        FUNC_NEQUAL,
        FUNC_LESS,
        FUNC_GREATER,
        FUNC_LEQUAL,
        FUNC_GEQUAL
    };
    
    enum CULL {
        CULL_FRONT = 0,
        CULL_BACK,
        CULL_NONE,
    };
    
    enum WINDING {
        WINDING_CW = 0,
        WINDING_CCW,
    };
    
    struct Caps {
        struct BindlessTextureCaps {
            uint64_t        m_supportsBindless : 1;
            uint64_t        m_useResourceHeap : 1;
            uint64_t        m_maxTextures;
            
            BindlessTextureCaps() {
                m_supportsBindless = 0;
                m_useResourceHeap = 0;
                m_maxTextures = 0;
            }
        };
                
        BindlessTextureCaps     m_bindlessTexture;
        
        Caps() {
            // Empty - does nothing
        }
    };
}}


#endif

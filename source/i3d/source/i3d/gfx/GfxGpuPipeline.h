//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXGPUPIPELINE_H__
#define __GFXGPUPIPELINE_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"
#include "i3d/gfx/GfxVertexDesc.h"

namespace i3d { namespace gfx {
    
    class Context;
    
    class GpuPipeline {
    public:
        struct Params {
            static constexpr size_t MAX_VERTEX_DESC = 8;
            
            const char *        m_vertexFuncName;
            const char *        m_pixelFuncName;
            VertexDesc *        m_vertexDesc[MAX_VERTEX_DESC];
            Context *           m_context;
            const char *        m_label;
            
            Params() {
                m_vertexFuncName = nullptr;
                m_pixelFuncName = nullptr;
                memset(m_vertexDesc, 0, sizeof(m_vertexDesc));
                m_context = nullptr;
                m_label = nullptr;
            }
        };
        
        virtual ~GpuPipeline() {
            // Empty - does nothing
        }
        
        virtual void Create( const Params * params ) = 0;
        
        virtual void Destroy() = 0;
    };
}}

#endif 

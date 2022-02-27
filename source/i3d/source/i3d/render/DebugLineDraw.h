//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_DEBUGLINEDRAW_METAL_H__
#define __I3D_DEBUGLINEDRAW_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/Gfx.h"
#include "i3d/render/RenderBuffers.h"

namespace i3d {

    class CmdScene;
    class CmdScene3d;

    class DebugLineDraw {
    public:
        struct DebugVertex {
            float       pos[4];
            float       colour[4];
        };
        
        size_t                          m_maxLines;
        size_t                          m_vertexBufferStride;        
        RenderBuffers *                 m_renderBuffers;
        gfx::GpuPipeline *              m_pipeline;
        gfx::VertexDesc *               m_vertexDesc;
        
        uint32_t                        m_vertexCapacity;
        uint32_t                        m_indexCapacity;
        
        DebugLineDraw();
        
        ~DebugLineDraw();
        
        void Initialise( RenderBuffers * renderBuffers, size_t maxLines );
        
        void DoPass( CmdScene3d * scene3d );
        
        void BuildLineBuffers( CmdScene * scene );
    };
}

#endif

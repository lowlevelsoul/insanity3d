//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __STATICTRANSFORM_METAL_H__
#define __STATICTRANSFORM_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/render/RenderBatchCmd.h"

#include "i3d/render/RenderBuffers.h"

#include "i3d/gfx/GfxGpuPipeline.h"
#include "i3d/gfx/GfxVertexDesc.h"

namespace i3d { namespace gfx {
    class GpuPipeline;
    class VertexDesc;
    class Context;
}}

namespace i3d {

    class BatchCmdScene;
    struct VertexFormat;

    class StaticTransform {
    public:
        StaticTransform();
        
        ~StaticTransform();
        
        void Initialise( RenderBuffers * buffers, bool batchByMaterial );
        
        void DoPass( CmdScene * scene );
        
        void BuildConstantBuffers( CmdScene3d * scene3d );
        
        void DoPassVertices( CmdScene * scene );
        
        void DoPassIndices( CmdScene * scene );
        
    public:
        bool                                m_batchMaterials;
        RenderBuffers *                     m_buffers;
        RenderBuffers::BufferInfo *         m_currFrame;
        size_t                              m_inputVertexStride;
        size_t                              m_outputVertexStride;

        gfx::GpuPipeline *                  m_vertexPipeline;
        gfx::GpuPipeline *                  m_indexPipeline;
        gfx::VertexDesc *                   m_inputVertexDesc;
        gfx::VertexDesc *                   m_inputIndexDesc;
        gfx::VertexDesc *                   m_outputVertexDesc;
        
        uintptr_t                           m_currOutVertexOffset;
        uintptr_t                           m_currIndexOffset;
        uint32_t                            m_currOutVertexCount;
    };
}

#endif

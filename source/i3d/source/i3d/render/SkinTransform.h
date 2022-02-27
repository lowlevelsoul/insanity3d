//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SKINTRANSFORM_METAL_H__
#define __SKINTRANSFORM_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/render/RenderBuffers.h"

#include "i3d/gfx/GfxGpuPipeline.h"
#include "i3d/gfx/GfxBuffer.h"

namespace i3d {

    class SkinTransform {
    public:
        
        SkinTransform();
        
        ~SkinTransform();
        
        void Initialise( RenderBuffers * buffers );
        
        void DoPass( CmdScene * scene );
        
        void DoPassSoftware( CmdScene * scene );
        
        void DoPassGpu( CmdScene * scene );
        
    protected:
        gfx::GpuPipeline *                  m_pipeline;
        gfx::VertexDesc *                   m_inputVertexDesc;
        gfx::VertexDesc *                   m_inputInfluenceDesc;
                
        size_t                              m_staticVertexSize;
        
        RenderBuffers *                m_buffers;
        RenderBuffers::BufferInfo *    m_currFrame;
    };
}

#endif

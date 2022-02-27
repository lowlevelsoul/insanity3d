//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXGPUPIPELINE_METAL_H__
#define __GFXGPUPIPELINE_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"
#include "i3d/gfx/GfxVertexDesc.h"
#include "i3d/gfx/GfxGpuPipeline.h"
#include <Metal/Metal.h>

namespace i3d { namespace gfx {
    
    class VertexDesc;
    class GpuFunctionMetal;
    
    class GpuPipelineMetal : public GpuPipeline {
    public:        
        GpuPipelineMetal();
        
        virtual ~GpuPipelineMetal();
        
        virtual void Create( const Params * params ) override;
        
        virtual void Destroy() override;
        
    public:
        id<MTLRenderPipelineState>  m_pipeline;
        MTLVertexDescriptor *       m_vertexDesc;
        GpuFunctionMetal *          m_vertexFunc;
        GpuFunctionMetal *          m_pixelFunc;
    };
}}

#endif

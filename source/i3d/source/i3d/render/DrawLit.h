//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_DRAWLIT_H__
#define __I3D_DRAWLIT_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/Gfx.h"
#include "i3d/gfx/GfxDepthState.h"
#include "i3d/gfx/GfxRasterizeState.h"
#include "i3d/gfx/GfxPass.h"
#include "i3d/render/RenderBuffers.h"

namespace i3d {

    class CmdScene;
    class CmdScene3d;
    class Material;

    class DrawLit {
    public:
        DrawLit();
        
        ~DrawLit();
        
        void Initialise( RenderBuffers * renderBuffers, gfx::Context * gfxCtxt );
        
        void BuildConstants( CmdScene * scene );
        
        void DoPass( CmdScene * scene );
        
        void DoPass( CmdScene3d * scene3d );
        
        void UseMaterial( i3d::Material * mat );
        
    public:
        gfx::RasterizeState *           m_rasterizeState;
        gfx::DepthState *               m_depthState;
        gfx::GpuPipeline *              m_pipeline;
        gfx::VertexDesc *               m_vertexDesc;
        
        RenderBuffers *                 m_renderBuffers;
        RenderBuffers::BufferInfo *     m_currBuffer;
    };
}

#endif

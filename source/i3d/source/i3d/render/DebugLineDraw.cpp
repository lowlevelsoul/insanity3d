//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/DebugLineDraw.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/gfx/GfxBuffer.h"
#include <string.h>

namespace i3d {
    
    static const gfx::VertexDesc::Element LINE_VERTEX_ELEMENTS[] = {
        {gfx::TYPE_FLOAT4,      0},         // Position
        {gfx::TYPE_FLOAT4,      16},        // Colour
        {gfx::TYPE_NIL }
    };
    
    static constexpr uint64_t Debug3dBufferIndex_Verts           = 0;
    static constexpr uint64_t Debug3dBufferIndex_Uniforms        = 1;

    typedef struct
    {
        Matrix4 projMatrix;
        Matrix4 viewMatrix;
    } Debug3dUniforms;
    
    extern CVar render_lineVertexCapacity;
    extern CVar render_lineIndexCapacity;

    //======================================================================================================================
    DebugLineDraw::DebugLineDraw() {
        m_pipeline = nullptr;
        m_vertexDesc = nullptr;
    }

    //======================================================================================================================
    DebugLineDraw::~DebugLineDraw() {
        
    }

    //======================================================================================================================
    void DebugLineDraw::Initialise( RenderBuffers * renderBuffers, gfx::Context * gfxCtxt ) {
        m_renderBuffers = renderBuffers;
        
        m_vertexDesc = renderBuffers->m_lineVertexDesc;     // gfx::CreateVertexDesc( LINE_VERTEX_ELEMENTS );
        m_vertexBufferStride = renderBuffers->m_lineVertexDesc->GetStride();
        
        // Create the vertex transform pipeline
        {
            gfx::GpuPipeline::Params plParams;
            plParams.m_vertexFuncName   = "Debug3dVertexShader";
            plParams.m_pixelFuncName    = "Debug3dFragmentShader";
            plParams.m_vertexDesc[0]    = m_vertexDesc;
            plParams.m_label            = "Debug Line Draw";
            plParams.m_context          = gfxCtxt;
            
            m_pipeline = gfx::CreateGpuPipeline( &plParams );
        }
    }
    
    //======================================================================================================================
    void DebugLineDraw::DoPass( CmdScene * scene ) {
        gfx::SetRenderPipeline( m_pipeline );
        gfx::SetVertexBuffer( m_renderBuffers->m_currBuffer->m_lineVertices, Debug3dBufferIndex_Verts, 0);
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head;
             scene3d != nullptr;
             scene3d = scene3d->m_next) {
            DoPass ( scene3d );
        }
    }

    //======================================================================================================================
    void DebugLineDraw::DoPass( CmdScene3d * scene3d ) {
        
#if 0
        // NEW LINE DRAWING CODE
        RenderBuffers::AllocInfo<Debug3dUniforms> sceneConstInfo;
        m_renderBuffers.AllocSceneConstant(sceneConstInfo, 1);

        Debug3dUniforms uniforms;
        sceneConstInfo.m_memory->projMatrix = scene->m_matProj;
        sceneConstInfo.m_memory->viewMatrix = scene->m_matView;
        
        gfx::SetVertexBuffer( m_renderBuffers.m_currBuffer->m_sceneConstantBuffer, Debug3dBufferIndex_Uniforms, sceneConstInfo.m_offs);
        
        gfx::DrawIndexedPrim( gfx::PRIM_LINES, scene->m_lineIndexStart,
                              scene->m_lineIndexCount, m_renderBuffers.m_currBuffer->m_lineIndices);
#endif
        
#if 1
                    
        if ( scene3d->m_lineIndexCount == 0 ) {
            return;
        }
        
        gfx::SetRenderPipeline( m_pipeline );
        gfx::SetVertexBuffer( m_renderBuffers->m_currBuffer->m_lineVertices, Debug3dBufferIndex_Verts, 0);
        
        RenderBuffers::AllocInfo<Debug3dUniforms> sceneConstInfo;
        m_renderBuffers->AllocSceneConstant(sceneConstInfo, 1);
        
        sceneConstInfo.m_memory->projMatrix = scene3d->m_matProj;
        sceneConstInfo.m_memory->viewMatrix = scene3d->m_matView;
                
        gfx::SetVertexBuffer( m_renderBuffers->m_currBuffer->m_sceneConstantBuffer, Debug3dBufferIndex_Uniforms, sceneConstInfo.m_offs );
                    
        /*
        size_t vertexDataSize = m_vertexBufferStride * vertexCount;
        size_t indexDataSize = sizeof(uint32_t) * indexCount;
        
        uint8_t * vertexMem = (uint8_t *) m_renderBuffers->m_currBuffer->m_lineVertices->Map();
        uint8_t * indexMem = (uint8_t *) m_renderBuffers->m_currBuffer->m_lineIndices->Map();
        
        uint8_t * bufferDst = ((uint8_t *) vertexMem);
        memcpy(bufferDst, verts, vertexDataSize );
        
        uint8_t * indexBufferDst = ((uint8_t *) indexMem );
        memcpy(indexBufferDst, indices, indexDataSize );
        
        m_renderBuffers->m_currBuffer->m_lineVertices->Unmap();
        m_renderBuffers->m_currBuffer->m_lineIndices->Unmap(); */
        
        gfx::DrawIndexedPrim( gfx::PRIM_LINES,
                              scene3d->m_lineIndexStart, scene3d->m_lineIndexCount,
                              m_renderBuffers->m_currBuffer->m_lineIndices );
#endif
    }
    
    //======================================================================================================================
    void DebugLineDraw::BuildLineBuffers( CmdScene * scene ) {
        
        if (scene->m_lineIndexCount == 0 ) {
            return;
        }
        
        DebugVertex * verts = (DebugVertex *) m_renderBuffers->m_currBuffer->m_lineVertices->Map();
        uint32_t * indices = (uint32_t *) m_renderBuffers->m_currBuffer->m_lineIndices->Map();
        
        memcpy(verts, scene->m_lineVerts, m_vertexBufferStride * scene->m_lineVertexCount );
        memcpy(indices, scene->m_lineIndices, sizeof(uint32_t) * scene->m_lineIndexCount );
        
        m_renderBuffers->m_currBuffer->m_lineVertices->Unmap();
        m_renderBuffers->m_currBuffer->m_lineIndices->Unmap();
    }
}

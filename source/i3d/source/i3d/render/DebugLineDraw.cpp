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
        m_maxLines = 0;
        m_pipeline = nullptr;
        m_vertexDesc = nullptr;
        m_vertexCapacity = 0;
        m_indexCapacity = 0;
    }

    //======================================================================================================================
    DebugLineDraw::~DebugLineDraw() {
        
    }

    //======================================================================================================================
    void DebugLineDraw::Initialise( RenderBuffers * renderBuffers, size_t maxLines ) {
        
        m_vertexCapacity = render_lineVertexCapacity.GetInt();
        m_indexCapacity = render_lineIndexCapacity.GetInt();
        
        m_renderBuffers = renderBuffers;
        
        m_vertexDesc = gfx::CreateVertexDesc( LINE_VERTEX_ELEMENTS );
        m_vertexBufferStride = m_vertexDesc->GetStride();
        
        // Create the vertex transform pipeline
        {
            gfx::GpuPipeline::Params plParams;
            plParams.m_vertexFuncName = "Debug3dVertexShader";
            plParams.m_pixelFuncName = "Debug3dFragmentShader";
            plParams.m_vertexDesc[0] = m_vertexDesc;
            plParams.m_label = "Debug Line Draw";
            
            m_pipeline = gfx::CreateGpuPipeline( &plParams );
        }
    
        m_maxLines = maxLines;
    }

    //======================================================================================================================
    void DebugLineDraw::DoPass( CmdScene3d * scene3d ) {
        
#if 0
        // NEW LINE DRAWING CODE
        
        [encoder setRenderPipelineState: this->m_lineDrawPipeline ];
        gfx::SetVertexBuffer( m_renderBuffers.m_currBuffer->m_lineVertices, Debug3dBufferIndex_Verts, 0);
        
    
        RenderBuffers::AllocInfo<Debug3dUniforms> sceneConstInfo;
        m_renderBuffers.AllocSceneConstant(sceneConstInfo, 1);

        Debug3dUniforms uniforms;
        sceneConstInfo.m_memory->projMatrix = scene->m_matProj;
        sceneConstInfo.m_memory->viewMatrix = scene->m_matView;
        
        gfx::SetVertexBuffer( m_renderBuffers.m_currBuffer->m_sceneConstantBuffer, Debug3dBufferIndex_Uniforms, sceneConstInfo.m_offs);
        
        gfx::DrawIndexedPrim( gfx::PRIM_LINES, scene->m_lineIndexStart,
                              scene->m_lineIndexCount, m_renderBuffers.m_currBuffer->m_lineIndices);
#endif
        
#if 0
                    
        if ( vertexCount == 0 ) {
            return;
        }
        
        gfx::SetRenderPipeline( m_pipeline );
        gfx::SetVertexBuffer( m_renderBuffers->m_currBuffer->m_lineVertices, Debug3dBufferIndex_Verts, 0);
        
        RenderBuffers::AllocInfo<Debug3dUniforms> sceneConstInfo;
        m_renderBuffers->AllocSceneConstant(sceneConstInfo, 1);
        
        sceneConstInfo.m_memory->projMatrix = projMat;
        sceneConstInfo.m_memory->viewMatrix = viewMat;
                
        gfx::SetVertexBuffer( m_renderBuffers->m_currBuffer->m_sceneConstantBuffer, Debug3dBufferIndex_Uniforms, sceneConstInfo.m_offs );
                    
        size_t vertexDataSize = m_vertexBufferStride * vertexCount;
        size_t indexDataSize = sizeof(uint32_t) * indexCount;
        
        uint8_t * vertexMem = (uint8_t *) m_renderBuffers->m_currBuffer->m_lineVertices->Map();
        uint8_t * indexMem = (uint8_t *) m_renderBuffers->m_currBuffer->m_lineIndices->Map();
        
        uint8_t * bufferDst = ((uint8_t *) vertexMem);
        memcpy(bufferDst, verts, vertexDataSize );
        
        uint8_t * indexBufferDst = ((uint8_t *) indexMem );
        memcpy(indexBufferDst, indices, indexDataSize );
        
        m_renderBuffers->m_currBuffer->m_lineVertices->Unmap();
        m_renderBuffers->m_currBuffer->m_lineIndices->Unmap();
        
        gfx::DrawIndexedPrim( gfx::PRIM_LINES, 0, indexCount, m_renderBuffers->m_currBuffer->m_lineIndices );
#endif
    }
    
    //======================================================================================================================
    void DebugLineDraw::BuildLineBuffers( CmdScene * scene ) {
        
        uint32_t currIndexCount = 0;
        uint32_t currVertexCount = 0;
        
        DebugVertex * verts = (DebugVertex *) m_renderBuffers->m_currBuffer->m_lineVertices->Map();
        uint32_t * indices = (uint32_t *) m_renderBuffers->m_currBuffer->m_lineIndices->Map();
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            scene3d->m_lineIndexStart = currIndexCount;
            
            if ( scene3d->m_lineIndexCount > 0 ) {
                
                uint32_t newVertexCount = (uint32_t) ( currVertexCount + scene3d->m_lineVertexCount );
                uint32_t newIndexCount = (uint32_t) ( currIndexCount + scene3d->m_lineIndexCount );
                
                if ( newIndexCount < m_indexCapacity && newVertexCount < m_vertexCapacity ) {
                    
                    memcpy( verts, scene3d->m_lineVerts, scene3d->m_lineVertexCount * sizeof(DebugVertex));
                    
                    for ( uint32_t i = 0; i < scene3d->m_lineIndexCount; i += 2) {
                        indices[i] = scene3d->m_lineIndices[i] + currVertexCount;
                        indices[i+1] = scene3d->m_lineIndices[i+1] + currVertexCount;
                    }
                }
                
                scene3d->m_lineIndexStart = currIndexCount;
                currIndexCount = newIndexCount;
                currVertexCount = newVertexCount;
            }
        }
        
        m_renderBuffers->m_currBuffer->m_lineVertices->Unmap();
        m_renderBuffers->m_currBuffer->m_lineIndices->Unmap();
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/RenderBuffers.h"
#include "i3d/gfx/Gfx.h"
#include "i3d/gfx/GfxBuffer.h"

namespace i3d {
    
    extern CVar render_gpuSkinning;
    
    extern CVar render_transformVertexConstantSize;
    extern CVar render_transformIndexConstantSize;
    extern CVar render_sceneConstantBufferSize;
    extern CVar render_lineVertexCapacity;
    extern CVar render_lineIndexCapacity;
    
    static const gfx::VertexDesc::Element LINE_VERTEX_ELEMENTS[] = {
        {gfx::TYPE_FLOAT4,      0},         // Position
        {gfx::TYPE_FLOAT4,      16},        // Colour
        {gfx::TYPE_NIL }
    };

    //======================================================================================================================
    RenderBuffers::RenderBuffers() {
        m_vertexBufferSize = 0;
        m_indexBufferSize = 0;
        m_skinTargetBufferSize = 0;
        m_nextBufferIndex = 0;
        m_skinUniformBufferSize = 0;
        m_constantOffsetAlign = 512;
        m_lineVertexDesc = nullptr;
    }

    //======================================================================================================================
    RenderBuffers::~RenderBuffers() {
        
    }

    //======================================================================================================================
    void RenderBuffers::Initialise( size_t vertexBufferSize, size_t indexBufferSize,
                                    size_t skinTargetBufferSize, size_t skinUniformBufferSize ) {
        
        XE_LOG( "Creating metal geometry render buffers (%u)\n", BUFFER_COUNT );
        
        m_lineVertexDesc = gfx::CreateVertexDesc( LINE_VERTEX_ELEMENTS );

        m_vertexBufferSize = vertexBufferSize;
        m_indexBufferSize = indexBufferSize;
        m_skinTargetBufferSize = skinTargetBufferSize;
        m_skinUniformBufferSize = skinUniformBufferSize;
        
        m_vertexTransformConstantBufferSize = render_transformVertexConstantSize.GetInt();
        m_indexTransformConstantBufferSize = render_transformIndexConstantSize.GetInt();
        m_sceneConstantBufferSize = render_sceneConstantBufferSize.GetInt();
        
        for ( int32_t i = 0; i < BUFFER_COUNT; ++i ) {
            
            BufferInfo & bi = m_buffers[ i ];
            
            bi.m_vertexBuffer = gfx::CreateBuffer( vertexBufferSize, gfx::BUFFER_GPU );
            bi.m_indexBuffer = gfx::CreateBuffer( indexBufferSize, gfx::BUFFER_GPU );
            bi.m_vertexTransformConstants = gfx::CreateBuffer( m_vertexTransformConstantBufferSize, 0 );
            bi.m_indexTransformConstants = gfx::CreateBuffer( m_indexTransformConstantBufferSize, 0 );
            
            bi.m_sceneConstantBuffer = gfx::CreateBuffer( m_sceneConstantBufferSize, 0 );
            
            if ( render_gpuSkinning.GetBool() == true ) {
                // GPU skinning. Allocate dest buffer as in private memory storage (GPU only)
                // and allocate a buffer to hold skin matrices.
                bi.m_skinTargetBuffer = gfx::CreateBuffer( m_skinTargetBufferSize, gfx::BUFFER_GPU );
                bi.m_skinUniforms = gfx::CreateBuffer( m_skinUniformBufferSize, 0 );
            }
            else {
                // Software skinning. We need an intermediate buffer that the CPU can
                // access to hold the results of skinned meshes
                bi.m_skinTargetBuffer = gfx::CreateBuffer( m_skinTargetBufferSize, 0 );
            }
            
#if 0
            if ( textureArrayBufferSize != 0 ) {
                bi.m_bindlessTextureArray = gfx::CreateBuffer( textureArrayBufferSize, 0 );
            }
            else {
                bi.m_bindlessTextureArray = nullptr;
            }
#endif
            
            bi.m_lineVertices = gfx::CreateBuffer( render_lineVertexCapacity.GetInt() * m_lineVertexDesc->GetStride(), 0 );
            bi.m_lineIndices = gfx::CreateBuffer( render_lineIndexCapacity.GetInt() * sizeof(uint32_t), 0 );
            
            bi.m_staticTransformFence = gfx::CreateFence( "Static Transform End" );
            bi.m_skinTransformFence = gfx::CreateFence( "Skin Transform End" );
        }
        
        m_nextBufferIndex = 0;
    }

    //======================================================================================================================
    void RenderBuffers::BeginFrame() {
                
        m_currBuffer = &m_buffers[ m_nextBufferIndex ];
        m_nextBufferIndex = ( ( m_nextBufferIndex + 1 ) % BUFFER_COUNT );
        
        m_currBuffer->m_vertexBufferOffs = 0;
        m_currBuffer->m_indexBufferOffs = 0;
        m_currBuffer->m_skinBufferOffs = 0;
        m_currBuffer->m_skinUniformOffs = 0;
        m_currBuffer->m_sceneConstantOffs = 0;
        m_currBuffer->m_vertexTransformOffs = 0;
        m_currBuffer->m_indexTransformOffs = 0;
        
        m_currBuffer->m_sceneConstantMem = (uint8_t *) m_currBuffer->m_sceneConstantBuffer->Map();
        m_currBuffer->m_skinConstantMem = (uint8_t *) m_currBuffer->m_skinUniforms->Map();
    }
    
    //======================================================================================================================
    void RenderBuffers::BeginConstants() {
        m_currBuffer->m_vertexTransformMem = (uint8_t*) m_currBuffer->m_vertexTransformConstants->Map( gfx::BUFFER_MAP_WRITEONLY );
        m_currBuffer->m_indexTransformMem = (uint8_t*) m_currBuffer->m_indexTransformConstants->Map( gfx::BUFFER_MAP_WRITEONLY );
        
        m_currBuffer->m_vertexTransformOffs = 0;
        m_currBuffer->m_indexBufferOffs = 0;
    }
    
    //======================================================================================================================
    void RenderBuffers::EndConstants() {
        m_currBuffer->m_vertexTransformConstants->Unmap();
        m_currBuffer->m_indexTransformConstants->Unmap();
        
        m_currBuffer->m_vertexTransformMem = nullptr;
        m_currBuffer->m_indexTransformMem = nullptr;
    }
}

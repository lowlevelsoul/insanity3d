//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/StaticTransform.h"
#include "i3d/render/Model_local.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/gfx/Gfx.h"

namespace i3d {
    
    extern CVar render_gpuSkinning;
    extern CVar render_skinning;
    
    static constexpr uint32_t BufferIndex_VertsDstBuffer      = 0;
    static constexpr uint32_t BufferIndex_VertsSrcBuffer      = 1;
    static constexpr uint32_t BufferIndex_VertsSceneConst     = 2;
    static constexpr uint32_t BufferIndex_VertsMeshConst      = 3;

    static constexpr uint32_t BufferIndex_IndexDstBuffer      = 0;
    static constexpr uint32_t BufferIndex_IndexSrcBuffer      = 1;
    static constexpr uint32_t BufferIndex_IndexMeshConst      = 2;
    
    typedef struct {
        Matrix4 projMatrix;
        Matrix4 viewMatrix;
    } TransformStaticVertexSceneConstants;

    // Unifmorm struct that is used by the draw vertex shader
    typedef struct {
        Matrix4     modelMatrix;
        uint32_t    materialInfo[4];
    } TransformStaticVertexMeshConstants;

    typedef struct
    {
        uint32_t    vertexOffset;
    } TransformStaticIndexConstants;
    
    static const gfx::VertexDesc::Element INPUT_VERTEX_ELEMENTS[] = {
        {gfx::TYPE_FLOAT4,      0},
        {gfx::TYPE_FLOAT4,      16},
        {gfx::TYPE_NIL }
    };
    
    static const gfx::VertexDesc::Element OUTPUT_VERTEX_ELEMENTS[] = {
        {gfx::TYPE_FLOAT4,      0},
        {gfx::TYPE_FLOAT4,      16},
        {gfx::TYPE_FLOAT4,      32},
        {gfx::TYPE_UINT4,       48},
        {gfx::TYPE_NIL }
    };
    
    static const gfx::VertexDesc::Element INPUT_INDEX_ELEMENTS[] = {
        {gfx::TYPE_UINT,      0},
        {gfx::TYPE_NIL }
    };

    //======================================================================================================================
    StaticTransform::StaticTransform() {
        m_buffers = nullptr;
        m_currFrame = nullptr;
        m_batchMaterials = false;
        
        m_vertexPipeline = nullptr;
        m_indexPipeline = nullptr;
        m_inputVertexDesc = nullptr;
        m_inputIndexDesc = nullptr;
        m_outputVertexDesc = nullptr;
    }

    //======================================================================================================================
    StaticTransform::~StaticTransform() {
        SAFE_DELETE( m_vertexPipeline );
        SAFE_DELETE( m_indexPipeline );
        SAFE_DELETE( m_inputIndexDesc );
        SAFE_DELETE( m_outputVertexDesc );
    }
    
    //======================================================================================================================
    void StaticTransform::DoPass( CmdScene * scene ) {
        DoPassVertices( scene );
        DoPassIndices( scene );
    }
    
    //======================================================================================================================
    void StaticTransform::Initialise( RenderBuffers * buffers, bool batchMaterials ) {
        m_batchMaterials = batchMaterials;
        m_buffers = buffers;
        
        m_currFrame = nullptr;

        m_inputVertexDesc = gfx::CreateVertexDesc( INPUT_VERTEX_ELEMENTS );
        m_inputIndexDesc = gfx::CreateVertexDesc( INPUT_INDEX_ELEMENTS );
        m_outputVertexDesc = gfx::CreateVertexDesc( OUTPUT_VERTEX_ELEMENTS );
        
        m_inputVertexStride = m_inputVertexDesc->GetStride();
        m_outputVertexStride = m_outputVertexDesc->GetStride();
        
        // Create the vertex transform pipeline
        {
            gfx::GpuPipeline::Params plParams;
            plParams.m_vertexFuncName = "TransformStatic_Vertex";
            plParams.m_vertexDesc[0] = m_inputVertexDesc;
            plParams.m_label = "Static Transform Vertex";
            
            m_vertexPipeline = gfx::CreateGpuPipeline( &plParams );
        }
        
        // Create the index transform pipeline
        {
            gfx::GpuPipeline::Params plParams;
            plParams.m_vertexFuncName = "TransformStatic_Index";
            plParams.m_vertexDesc[0] = m_inputIndexDesc;
            plParams.m_label = "Static Transform Index";
            
            m_indexPipeline = gfx::CreateGpuPipeline( &plParams );
        }
    }
    
    //======================================================================================================================
    void StaticTransform::DoPassVertices( CmdScene * scene ) {
        
        // Setup pipeline and the buffers used for the entire duration of the pipeline
        gfx::SetRenderPipeline( m_vertexPipeline );
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_vertexBuffer, BufferIndex_VertsDstBuffer, 0 );
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_sceneConstantBuffer, BufferIndex_VertsSceneConst, 0 );
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_sceneConstantBuffer,  BufferIndex_VertsMeshConst, 0 );
        
        uint32_t vertexDrawCount = 0;
                
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            
            // Scene the scene constants for this scene transform step
            RenderBuffers::AllocInfo<TransformStaticVertexSceneConstants> sceneConst;
            m_buffers->AllocSceneConstant(sceneConst, 1);
            
            sceneConst.m_memory->viewMatrix = scene3d->m_matView;
            sceneConst.m_memory->projMatrix = scene3d->m_matProj;
            gfx::SetVertexBufferOffset( BufferIndex_VertsSceneConst, sceneConst.m_offs );

            CmdMaterial * matCmd = &scene->m_materials[ scene3d->m_materialListStart ];
            
            // Loop through all of the naterial batches in this scene
            for ( uint32_t imat = 0; imat < scene3d->m_materialListCount; ++ imat ) {
                
                for ( CmdDrawModel * draw = matCmd->m_drawList.m_head; draw != nullptr; draw = draw->m_next ) {
                    
                    draw->m_transformVertexStart = vertexDrawCount;
                    
                    ModelLocal * modelLocal = (ModelLocal *) draw->m_model;
                    
                    
                    // Set the transform constants for this mesh
                    uintptr_t dstVertexOffset = 0;
                    m_buffers->AllocVertex( dstVertexOffset, m_outputVertexStride * draw->m_model->GetVertexCount() );
   
                    RenderBuffers::AllocInfo<TransformStaticVertexMeshConstants> meshConst;
                    m_buffers->AllocSceneConstant( meshConst, 1 );
                    meshConst.m_memory->modelMatrix = draw->m_transform;
                    meshConst.m_memory->materialInfo[0] = 0;
                    meshConst.m_memory->materialInfo[1] = 0;
                    meshConst.m_memory->materialInfo[2] = 0;
                    meshConst.m_memory->materialInfo[3] = 0;
                    
                    // Set the offset for the destination / transformed vertices
                    gfx::SetVertexBufferOffset( BufferIndex_VertsDstBuffer, dstVertexOffset );
                    
                    // Set the offset for the mesh info
                    gfx::SetVertexBufferOffset( BufferIndex_VertsMeshConst, meshConst.m_offs );
                    
                    // Set the src vertex buffer
                    gfx::SetVertexBuffer(modelLocal->m_vertices, BufferIndex_VertsSrcBuffer, 0 );
                    
                    // Tansform the vertices
                    gfx::DrawPrim( gfx::PRIM_POINTS, 0, draw->m_model->GetVertexCount() );
                    
                    vertexDrawCount += draw->m_model->GetVertexCount();
                }
                
                ++matCmd;
            }
        }
    }
    
    //======================================================================================================================
    void StaticTransform::DoPassIndices( CmdScene * scene ) {
        
        // Setup pipeline and the buffers used for the entire duration of the pipeline
        gfx::SetRenderPipeline( m_indexPipeline );
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_indexBuffer, BufferIndex_IndexDstBuffer, 0 );
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_sceneConstantBuffer, BufferIndex_IndexMeshConst, 0);

        uintptr_t totalindexCount = 0;
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            
            CmdMaterial * matCmd = &scene->m_materials[ scene3d->m_materialListStart ];
            
            
            for ( uint32_t imat = 0; imat < scene3d->m_materialListCount; ++ imat ) {
                
                matCmd->m_drawIndexStart = (uint32_t) totalindexCount;
                
                for ( CmdDrawModel * draw = matCmd->m_drawList.m_head; draw != NULL; draw = draw->m_next ) {
                    
                    ModelLocal * modelLocal = (ModelLocal *) draw->m_model;
                    uint32_t thisIndexCount = (uint32_t) modelLocal->GetIndexCount();
                    
                    uintptr_t indexOffset;
                    m_buffers->AllocIndex( indexOffset, sizeof(uint32_t) * thisIndexCount );
                
                    // Setup the constants for this
                    RenderBuffers::AllocInfo<TransformStaticIndexConstants> meshConst;
                    m_buffers->AllocSceneConstant( meshConst, 1 );
                    
                    meshConst.m_memory->vertexOffset = draw->m_transformVertexStart;
                    
                    gfx::SetVertexBufferOffset( BufferIndex_IndexMeshConst, meshConst.m_offs );
                    gfx::SetVertexBufferOffset( BufferIndex_IndexDstBuffer, indexOffset );
                    
                    gfx::SetVertexBuffer( modelLocal->m_indices, BufferIndex_IndexSrcBuffer, 0 );
                    gfx::DrawPrim( gfx::PRIM_POINTS, 0, thisIndexCount );
                    
                    totalindexCount += thisIndexCount;
                    matCmd->m_drawIndexCount += thisIndexCount;
                    

                }
                
                ++matCmd;
            }
        }
    }
    
    //======================================================================================================================
    void StaticTransform::BuildConstantBuffers( CmdScene3d * scene3d ) {
#if 0
        bool doSkinning = render_skinning.GetBool();
        
        for ( CmdDrawModel * draw = scene3d->m_modelDraw.m_head; draw != nullptr; draw = draw->m_next ) {
            
            if ( draw->m_skin != nullptr && doSkinning == true ) {
                
            }
            else {
                
            }
        }
#endif
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/SkinTransform.h"
#include "i3d/render/Model_local.h"
#include "i3d/render/ModelInstance.h"
#include "i3d/render/Skeleton.h"
#include "i3d/gfx/Gfx.h"
#include "i3d/core/CVar.h"

namespace i3d {
    
    extern CVar render_gpuSkinning;
    extern CVar render_skinning;
    
    static constexpr uint32_t TransformSkinBuffer_DstVerts            = 0;
    static constexpr uint32_t TransformSkinBuffer_SrcStaticVerts      = 1;
    static constexpr uint32_t TransformSkinBuffer_SrcWeights          = 2;
    static constexpr uint32_t TransformSkinBuffer_JointMats           = 3;
    static constexpr uint32_t TransformSkinBuffer_BindMats            = 4;
    
    #define SKIN_MATRICES_MAX 96
    
    static const gfx::VertexDesc::Element INPUT_VERTEX_ELEMENTS[] = {
        { gfx::TYPE_FLOAT4,      0 },
        { gfx::TYPE_FLOAT4,      16 },
        { gfx::TYPE_NIL }
    };
    
    static const gfx::VertexDesc::Element INPUT_INFLUENCE_ELEMENTS[] = {
        { gfx::TYPE_UINT4,       0 },
        { gfx::TYPE_FLOAT4,      16 },
        { gfx::TYPE_UINT4,       32 },
        { gfx::TYPE_NIL }
    };

    //======================================================================================================================
    SkinTransform::SkinTransform() {
        m_buffers = nullptr;
        m_staticVertexSize = 32;
        m_currFrame = nullptr;
        m_pipeline = nullptr;
        
        m_inputVertexDesc = nullptr;
        m_inputInfluenceDesc = nullptr;
    }

    //======================================================================================================================
    SkinTransform::~SkinTransform() {
        
    }

    //======================================================================================================================
    void SkinTransform::Initialise( RenderBuffers * buffers ) {
        m_buffers = buffers;
        m_currFrame = nullptr;
        m_staticVertexSize = 32;
        
        // Create the vertex transform pipeline
        {
            m_inputVertexDesc = gfx::CreateVertexDesc( INPUT_VERTEX_ELEMENTS );
            m_inputInfluenceDesc = gfx::CreateVertexDesc( INPUT_INFLUENCE_ELEMENTS );
            
            gfx::GpuPipeline::Params plParams;
            plParams.m_vertexFuncName = "TransformSkin_Vertex";
            plParams.m_vertexDesc[0] = m_inputVertexDesc;
            plParams.m_vertexDesc[1] = m_inputInfluenceDesc;
            plParams.m_label = "Skin Transform";
            
            m_pipeline = gfx::CreateGpuPipeline( &plParams );
        }
    }

    //======================================================================================================================
    void SkinTransform::DoPass( CmdScene * scene ) {
        
        bool doGpu = render_gpuSkinning.GetBool();
        if ( doGpu == true ) {
            DoPassGpu( scene );
        }
        else {
            DoPassSoftware( scene );
        }
    }
    
    //======================================================================================================================
    void SkinTransform::DoPassSoftware( CmdScene * scene ) {
        
#if 0
        uint32_t currDstVertexCount = 0;
        m_currFrame = m_buffers->m_currBuffer;
        
        ModelMetal::StaticVertex * dstVerts = ( ModelMetal::StaticVertex * ) m_currFrame->m_skinTargetBuffer.contents;
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            
            for (CmdSkinTransform * skin = scene3d->m_skins.m_head; skin != nullptr; skin = skin->m_next) {
                
                ModelMetal * modelMetal = (ModelMetal *) skin->m_modelInst->GetModel();
                
                ModelMetal::StaticVertex * skinVerts = (ModelMetal::StaticVertex *) modelMetal->m_vertices.contents;
                ModelMetal::Influence * influences = (ModelMetal::Influence *) modelMetal->m_influences.contents;
        
                skin->m_vertexStart = currDstVertexCount;
                skin->m_vertexCount = 0;
                
                const Matrix4 * bodyMats = skin->m_modelInst->GetBodyMatrices();
                const Matrix4 * bindMats = skin->m_modelInst->GetSkeleton()->GetJointBindMatrices();
                
                for ( CmdDrawModelMesh * mesh = &skin->m_meshes[0]; mesh != &skin->m_meshes[skin->m_meshCount]; ++mesh ) {
                    
                    ModelMetal::StaticVertex * skinVertsMesh = &skinVerts[mesh->m_vertexStart];
                    ModelMetal::Influence * influencesMesh = &influences[mesh->m_vertexStart];
                    
                    for ( uint32_t v = 0; v < mesh->m_vertexCount; ++v ) {
                        
                        Vector3 posAccum = Vector3::ZERO;
                        Vector3 normAccum = Vector3::ZERO;
                        
                        assert(influencesMesh->m_params[0] > 0 && influencesMesh->m_params[0] <= 4);
                        
                        for ( uint32_t i = 0; i < influencesMesh->m_params[0]; ++i ) {
                            uint32_t joint = influencesMesh->m_joints[i];
                            
                            Vector4 localP = bindMats[ joint ] * Vector4(skinVertsMesh->posTexU[0], skinVertsMesh->posTexU[1], skinVertsMesh->posTexU[2], 1 );
                            Vector4 localN = bindMats[ joint ] * Vector4(skinVertsMesh->normalTexV[0], skinVertsMesh->normalTexV[1], skinVertsMesh->normalTexV[2], 0 );
                            
                            Vector4 bodyP = bodyMats[ joint ] * localP;
                            Vector4 bodyN = bodyMats[ joint ] * localN;
                            
                            posAccum += ( Vector3( bodyP ) * influencesMesh->m_weights[ i ] );
                            normAccum += Vector3( bodyN );
                        }
                        
                        normAccum.Normalise();
                        dstVerts->posTexU[0] = posAccum.X();
                        dstVerts->posTexU[1] = posAccum.Y();
                        dstVerts->posTexU[2] = posAccum.Z();
                        dstVerts->posTexU[3] = skinVertsMesh->posTexU[3];
                        
                        dstVerts->normalTexV[0] = normAccum.X();
                        dstVerts->normalTexV[1] = normAccum.Y();
                        dstVerts->normalTexV[2] = normAccum.Z();
                        dstVerts->normalTexV[3] = skinVertsMesh->normalTexV[3];
                        
                        ++dstVerts;
                        ++skinVertsMesh;
                        ++influencesMesh;

                    }
                    
                    skin->m_vertexCount += mesh->m_vertexCount;
                    currDstVertexCount += mesh->m_vertexCount;
                }
            }
        }
#endif
    }
    
    //======================================================================================================================
    void SkinTransform::DoPassGpu( CmdScene * scene ) {
        
        uint32_t currDstVertexCount = 0;
        m_currFrame = m_buffers->m_currBuffer;
        
        gfx::SetRenderPipeline( m_pipeline );
        
        // Set skin target buffer
        gfx::SetVertexBuffer( m_currFrame->m_skinTargetBuffer, 0, 0 );

        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_skinUniforms, TransformSkinBuffer_BindMats, 0);
        gfx::SetVertexBuffer( m_buffers->m_currBuffer->m_skinUniforms, TransformSkinBuffer_JointMats, 0);

        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            
            for (CmdSkinTransform * skin = scene3d->m_skins.m_head; skin != nullptr; skin = skin->m_next) {
                
                ModelLocal * model = (ModelLocal *) skin->m_modelInst->GetModel();
                
                gfx::SetVertexBuffer( model->m_vertices, TransformSkinBuffer_SrcStaticVerts, 0 );
                gfx::SetVertexBuffer( model->m_influences, TransformSkinBuffer_SrcWeights, 0 );
                                
                skin->m_vertexStart = currDstVertexCount;
                skin->m_vertexCount = 0;
                
                RenderBuffers::AllocInfo<Matrix4> animMatInfo;
                RenderBuffers::AllocInfo<Matrix4> bindMatInfo;
                                
                m_buffers->AllocSkinConstant<Matrix4>( animMatInfo, SKIN_MATRICES_MAX );
                m_buffers->AllocSkinConstant<Matrix4>( bindMatInfo, SKIN_MATRICES_MAX );
                
                skin->m_offsAnimMatConst = animMatInfo.m_offs;
                skin->m_offsBindMatConst =bindMatInfo.m_offs;
                                
                const Matrix4 * bodyMats = skin->m_modelInst->GetBodyMatrices();
                const Matrix4 * bindMats = skin->m_modelInst->GetSkeleton()->GetJointBindMatrices();
                const size_t matrixDataSize = sizeof(Matrix4) * skin->m_modelInst->GetSkeleton()->GetJointCount();
                
                memcpy( animMatInfo.m_memory, bodyMats, matrixDataSize );
                memcpy( bindMatInfo.m_memory, bindMats, matrixDataSize );
                
                gfx::SetVertexBufferOffset( TransformSkinBuffer_BindMats, bindMatInfo.m_offs );
                gfx::SetVertexBufferOffset( TransformSkinBuffer_JointMats, animMatInfo.m_offs );
                
                for ( CmdDrawModelMesh * mesh = &skin->m_meshes[0]; mesh != &skin->m_meshes[skin->m_meshCount]; ++mesh ) {
                    
                    uintptr_t srcVertexOffset = mesh->m_vertexStart * sizeof( ModelLocal::StaticVertex);
                    uintptr_t srcInfluenceOffset = mesh->m_vertexStart * sizeof( ModelLocal::Influence);
                    uintptr_t dstVertexOffset = currDstVertexCount * sizeof( ModelLocal::StaticVertex );
                    
                    gfx::SetVertexBufferOffset( TransformSkinBuffer_DstVerts, dstVertexOffset );
                    gfx::SetVertexBufferOffset( TransformSkinBuffer_SrcStaticVerts, srcVertexOffset );
                    gfx::SetVertexBufferOffset( TransformSkinBuffer_SrcWeights, srcInfluenceOffset );
                    
                    gfx::DrawPrim( gfx::PRIM_POINTS, 0, mesh->m_vertexCount );
                                        
                    mesh->m_vertexStart = currDstVertexCount;
                    currDstVertexCount += mesh->m_vertexCount;
                    skin->m_vertexCount += mesh->m_vertexCount;
                }
            }
        }
    }
}

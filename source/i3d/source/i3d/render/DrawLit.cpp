//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/DrawLit.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/render/Render_local.h"

namespace i3d {

    static constexpr uint32_t Draw3dFragmentBuffer_Scene = 0;
    static constexpr uint32_t Draw3dFragmentBuffer_TextureArgs = 1;
    
    typedef struct {
        Matrix4 matProj;
        Matrix4 matView;
    } Draw3dSceneConstants;

    typedef struct {
        Matrix4 modelMatrix;
    } Draw3dModelConstants;

    typedef struct {
        Vector4     cameraPos;
        Vector4     globalLightDir;
        Vector4     globalLightColour;
    } Draw3dFragmentScene;
    
    static const gfx::VertexDesc::Element DRAW_LIT_VERTEX_DESC[] = {
        { gfx::TYPE_FLOAT4,             0},     // Clip position
        { gfx::TYPE_FLOAT4,            16},     // World position + tex u
        { gfx::TYPE_FLOAT4,            32},     // World normal + tex v
        { gfx::TYPE_UINT4,             48},     // Material texture indices
        { gfx::TYPE_NIL }
    };

    //======================================================================================================================
    DrawLit::DrawLit() {
        m_renderBuffers = nullptr;
        m_currBuffer = nullptr;
        m_pipeline = nullptr;
        m_vertexDesc = nullptr;
        m_rasterizeState = nullptr;
        m_depthState = nullptr;
    }

    //======================================================================================================================
    DrawLit::~DrawLit() {
        SAFE_DELETE( m_vertexDesc );
        SAFE_DELETE( m_rasterizeState );
        SAFE_DELETE( m_depthState );
    }

    //======================================================================================================================
    void DrawLit::Initialise( RenderBuffers * renderBuffers, gfx::Context * gfxCtxt ) {
        
        m_renderBuffers = renderBuffers;
        m_currBuffer = nullptr;
        
        // Create the pipeline for the lit rendering pass
        {
            m_vertexDesc = gfx::CreateVertexDesc( DRAW_LIT_VERTEX_DESC );
            
            gfx::GpuPipeline::Params plparams;
            plparams.m_vertexFuncName = "Draw3d_Pbr_VertexShader";     // "Draw3d_Toon_Bindless_VertexShader";
            plparams.m_pixelFuncName = "Draw3d_Pbr_FragmentShader";    // Draw3d_Toon_Bindless_FragmentShader";
            plparams.m_vertexDesc[0] = m_vertexDesc;
            plparams.m_context = gfxCtxt;
            
            m_pipeline = gfx::CreateGpuPipeline( &plparams );
        }
        
        m_depthState = gfx::CreateDepthState( gfx::FUNC_LESS, true, true, 1.0f, 0);
        m_rasterizeState = gfx::CreateRasterizeState( gfx::WINDING_CW, gfx::CULL_BACK );
    }
    
    //======================================================================================================================
    void DrawLit::BuildConstants( CmdScene * scene ) {
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
        
            RenderBuffers::AllocInfo<Draw3dFragmentScene> sceneFragment;
            m_renderBuffers->AllocSceneConstant(sceneFragment, 1);
        
            scene3d->m_scenePixelConstOffs = sceneFragment.m_offs;
        
            memcpy(&sceneFragment.m_memory->cameraPos, &scene3d->m_cameraPos, sizeof(Vector4));
            memcpy(&sceneFragment.m_memory->globalLightDir, &scene3d->m_globalLightDir, sizeof(Vector4));
            memcpy(&sceneFragment.m_memory->globalLightColour, &scene3d->m_globalLightColour, sizeof(Vector4));
        }
    }

    //======================================================================================================================
    void DrawLit::DoPass( CmdScene * scene ) {
        
        m_currBuffer = m_renderBuffers->m_currBuffer;
        
        gfx::SetRenderPipeline( m_pipeline );
        gfx::SetDepthState( m_depthState );
        gfx::SetRasterizeState( m_rasterizeState );

        gfx::SetVertexBuffer( m_currBuffer->m_vertexBuffer, 0, 0 );
        gfx::SetPixelBuffer( m_currBuffer->m_sceneConstantBuffer, Draw3dFragmentBuffer_Scene, 0 );
        
        for ( CmdScene3d * scene3d = scene->m_scene3dList.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            gfx::SetPixelBuffer( m_currBuffer->m_sceneConstantBuffer, Draw3dFragmentBuffer_Scene, scene3d->m_scenePixelConstOffs );
            
            CmdMaterial * matCmd = scene->m_materials + scene3d->m_materialListStart;
            
            for ( uint32_t imat = 0; imat < scene3d->m_materialListCount; ++imat ) {
                renderLocal->UseMaterial( matCmd->m_material );
                gfx::DrawIndexedPrim( gfx::PRIM_TRIANGLES, matCmd->m_drawIndexStart, matCmd->m_drawIndexCount , m_currBuffer->m_indexBuffer );
                ++matCmd;
            }
        }
    }
}

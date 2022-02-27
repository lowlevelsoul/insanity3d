//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx.h"
#include "i3d/gfx/GfxContext.h"
#include "i3d/gfx/GfxDepthState.h"
#include "i3d/gfx/GfxRasterizeState.h"

#include "i3d/math/Math3d.h"
#include "i3d/render/Render.h"
#include "i3d/render/Model.h"
#include "i3d/render/Material.h"
#include "i3d/render/Texture.h"
#include "i3d/render/Texture_Base.h"
#include "i3d/core/Log.h"

//#include "i3d/mem/Mem_local.h"
#include "i3d/render/RenderBatch.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/render/Render_local.h"
#include "i3d/render/DebugLineDraw.h"

#include "i3d/render/Model_local.h"
#include "i3d/render/Skeleton_local.h"

i3d::Render * render = nullptr;

namespace i3d {
    
    CVar render_skinning                        ( "render_skinning",                true,               "Enables / disables skinning" );
    CVar render_gpuSkinning                     ( "render_gpuSkinning",             true,               "Enables disable CPU skinning" );
    CVar render_mode                            ( "render_mode",                    1,                  "Render mode: 0 = PBR, 1 = toon" );
    CVar render_transformVertexConstantSize     ( "render_transformVertexConstantSize", 64 * 4096,      "Size of the constant buffer used for holding constants used when trabsforming verts" );
    CVar render_transformIndexConstantSize      ( "render_transformIndexConstantSize", 16 * 4096,       "Size of the constant buffer used for holding constants used when trasforming model indices" );
    CVar render_sceneConstantBufferSize         ( "render_sceneConstantBufferSize", 64 * 4096,          "Size of the buffer for scene constants");
    
    CVar render_lineVertexCapacity              ( "render_lineVertexCapacity",      4096,               "Max number of vertices for drawing lines");
    CVar render_lineIndexCapacity               ( "render_lineIndexCapacity",       4096,               "Max number of indices for drawing lines");
    CVar render_forceMaterialBatching           ( "render_forceMaterialBatching",   false,              "Forces the renderer to use material batching");

    RenderLocal * renderLocal = nullptr;
    
    struct LineVertex {
        float       pos[4];
        float       colour[4];
        
        void SetPosition( const Vector3 & p ) {
            pos[0] = p.X();
            pos[1] = p.Y();
            pos[2] = p.Z();
            pos[3] = 1;
        }
        
        void SetColour( const Vector4 & c ) {
            colour[0] = c.X();
            colour[1] = c.Y();
            colour[2] = c.Z();
            colour[2] = c.W();
        }
    };

    //======================================================================================================================
    void RenderCreate( RenderParams * params ) {
        if ( ::render != nullptr ) {
            return;
        }
        
        gfx::Initialise();
        
        ::render = new RenderLocal;
        renderLocal = (RenderLocal*) ::render;
        
        ::render->Initialise( params );
    }
    
    //======================================================================================================================
    void RenderDestroy() {
        if ( ::render == nullptr ) {
            return;
        }
        
        ::render->Finalise();
        ::render = nullptr;
        renderLocal = nullptr;
    }

    //======================================================================================================================
    RenderLocal::RenderLocal( ) {
        m_currScene     = nullptr;
        m_useTextureHeap = true;
        m_useBindlessTextures = false;
        m_useMaterialBatching = true;
        
        m_gfxContext = nullptr;
    }
    
    //======================================================================================================================
    RenderLocal::~RenderLocal() {
        
    }
    
    //======================================================================================================================
    void RenderLocal::Initialise( RenderParams * params ) {
        
        XE_LOG("=========================================\n");
        XE_LOG("RenderSystem init \n");
        XE_LOG("=========================================\n");
               
        // Create the gfx context
        {
            m_gfxContext = gfx::CreateContext();
            
            gfx::Context::Params gfxCtxtParams;
            gfxCtxtParams.m_bufferCount = 3;
            gfxCtxtParams.m_nativeView = params->m_nativeView;
            
            m_gfxContext->Create( gfxCtxtParams );
        }
        
        XE_ASSERT( m_useMaterialBatching  != m_useBindlessTextures );
        XE_LOG(" Using material batching : %s\n", ( m_useMaterialBatching == true) ? "YES" : "NO" );
        XE_LOG(" Using bindless textures : %s\n", ( m_useBindlessTextures == true) ? "YES" : "NO" );
        
        m_renderBuffers.Initialise( 1024 * 1024 * 16, 1024 * 1024 * 4, 1024 * 1024 * 4, 1024 * 1024 * 4);
        m_batcher.Initialise( 1024 * 1024 * 2, m_useMaterialBatching );
        
        res->PublishFactory<i3d::ModelLocal>();
        res->PublishFactory<i3d::TextureBase>();
        //res->PublishFactory<xe::SkeletonLocal>();

        CreateDefaultTextures();
        
        m_staticTransformPass = gfx::CreatePass( "Static Transform");
        //m_skinTransformPass = gfx::CreatePass( "Skin Transform");
        m_drawTransformPass = gfx::CreatePass( "Draw");
        
        m_drawLit.Initialise( &m_renderBuffers, m_gfxContext );
        //m_lineDraw.Initialise( &m_renderBuffers, 0 );
    
        m_staticTransform.Initialise( & m_renderBuffers, m_useMaterialBatching );
        //m_skinTransform.Initialise( &m_renderBuffers );
    }
    
    //======================================================================================================================
    void RenderLocal::CreateDefaultTextures() {
        m_defaultTextures[DEFAULT_TEXTURE_RED]     = ( Texture * ) res->Load( "~/textures/system/red.tex");
        m_defaultTextures[DEFAULT_TEXTURE_BLACK]   = ( Texture * ) res->Load( "~/textures/system/black.tex");
        m_defaultTextures[DEFAULT_TEXTURE_GREY]    = ( Texture * ) res->Load( "~/textures/system/grey.tex");
        m_defaultTextures[DEFAULT_TEXTURE_WHITE]   = ( Texture * ) res->Load( "~/textures/system/white.tex");
        m_defaultTextures[DEFAULT_TEXTURE_AMR]     = nullptr;  // CreateAmrTexture(0.3f, 0, 0.1f);
    }
    
    //======================================================================================================================
    handle_t RenderLocal::CreateAmrTexture( float ao, float metallic, float roughness ) {
#if 0
        uint8_t pixelRgba[4];
        
        pixelRgba[0] = (uint8_t) 255.0f * ao;
        pixelRgba[1] = (uint8_t) 255.0f * metallic;
        pixelRgba[2] = (uint8_t) 255.0f * roughness;
        pixelRgba[3] = 255;
        
        
        uint8_t pixels[64 * 64 * 4];
        
        for ( uint32_t i=0; i < sizeof(pixels); i += 4) {
            pixels[i] = pixelRgba[0];
            pixels[i+1] = pixelRgba[1];
            pixels[i+2] = pixelRgba[2];
            pixels[i+3] = pixelRgba[3];
        }
        
        handle_t htex = texture::AllocAndCreate( texture::FORMAT_RGBA_U8, 64, 64, 0 );
        texture::Write( htex, pixels, 0 );
        
        return htex;
#else
        return 0;
#endif
    }

    //======================================================================================================================
    void RenderLocal::Finalise() {
    }
    
    //======================================================================================================================
    void RenderLocal::BeginScene() {
        m_currScene = m_batcher.BeginScene();
    }

    //======================================================================================================================
    void RenderLocal::EndScene() {
        m_batcher.EndScene();
    }

    //======================================================================================================================
    void RenderLocal::BeginScene3d( const Matrix4& matProj, const Matrix4& matView, int32_t * viewport ) {
        m_batcher.BeginScene3d( matProj, matView, viewport );
    }

    //======================================================================================================================
    void RenderLocal::EndScene3d( ) {
        m_batcher.EndScene();
    }

    //======================================================================================================================
    void RenderLocal::SubmitModel( Model * model, const Matrix4& transform, Material * material ) {
        m_batcher.SubmitModel( model, transform, material );
        
    }

    //======================================================================================================================
    void RenderLocal::SubmitGlobalLight( const Vector3& dir, const Vector4& colour ) {
        
    }

    //======================================================================================================================
    void RenderLocal::SubmitDirectionLight( const Vector3& pos, const Vector3& dir, const Vector4& colour ) {
        
    }

    //======================================================================================================================
    void RenderLocal::SubmitPointLight( const Vector3& pos, float radius, float fallOff, const Vector4& colour ) {
        
    }
    
    //======================================================================================================================
    void RenderLocal::DrawLine( const Vector3 & v0, const Vector3 & v1, const Vector4 & colour ) {
        m_batcher.DrawLine( v0, v1, colour );
    }

    //======================================================================================================================
    void RenderLocal::BuildTexturePoolForFrame() {
    }
    
    //======================================================================================================================
    void RenderLocal::BuildSceneConstants( CmdScene * scene ) {
        m_drawLit.BuildConstants( scene );
    }
    
    //======================================================================================================================
    bool RenderLocal::SubmitBeginScene() {
        
        bool gfxSceneOk = gfx::BeginScene( m_gfxContext );
        if ( gfxSceneOk == false ) {
            return false;
        }
        
        if ( m_currScene != nullptr ) {
        
            // Get current set of buffers for this frame
            m_renderBuffers.BeginFrame();
        
            // Build the constant buffers for the scene. We do this in one go because
            // some gfx api's (e.g. OpenGL) end up having really weird usage requirments like
            // haing to map / unmap  buffers. Having the scene constant buffers built in one place
            // reduces the number of map's unmaps that we have to perform.
            BuildSceneConstants( m_currScene );
        
        // If we're using bindless textures, we need to build the texture "pool" buffer
        // that the rendering passes use. This is just a buffer that contains the address
        // of all the textures in GPU-space (or however the underlying gfx api wants them).
//        if (m_useBindlessTextures == true ) {
//            BuildTexturePoolForFrame();
//        }
        }
        
        return true;
    }

    //======================================================================================================================
    void RenderLocal::Submit() {

        
        bool sceneOk = SubmitBeginScene();
        if ( sceneOk == true ) {
            
            bool doSkinning = render_skinning.GetBool();
            bool doGpuSkinning = render_gpuSkinning.GetBool();
            
            // Build the buffers for drawing any lines
            //m_lineDraw.BuildLineBuffers( m_currScene );
            
            if ( m_currScene != nullptr ) {
           
                // Static transform pass - transform all modelspace vertcs into the transform buffers and
                // pack all of the indiced for those models into stream by material
                {
                    gfx::BeginPass(" Static Transform");
                    
                    m_staticTransform.DoPass( m_currScene );
                    gfx::UpdateFence( m_renderBuffers.m_currBuffer->m_staticTransformFence, gfx::RENDER_STAGE_VERTEX );
                    
                    gfx::EndPass();
                }

                {
                    gfx::BeginPass("Draw Lit 3D");
                    
                    if ( m_currScene != nullptr ) {
                        gfx::WaitForFence( m_renderBuffers.m_currBuffer->m_staticTransformFence, gfx::RENDER_STAGE_VERTEX );
                        m_drawLit.DoPass( m_currScene );
                    }
                    
                    gfx::EndPass();
                }
            }

            gfx::Present();
        }

        gfx::EndScene();
    }
    
    //======================================================================================================================
    void RenderLocal::DoDraw( CmdScene * scene ) {
        CmdScene3d::list_t & list3d = m_currScene->m_scene3dList;
    
        for ( CmdScene3d * scene3d = list3d.m_head; scene3d != nullptr; scene3d = scene3d->m_next ) {
            DoDraw3dOpaque( scene3d );
        }
    }
    
    //======================================================================================================================
    void RenderLocal::DoDraw3dOpaque( CmdScene3d * scene ) {
        
#if 0
        m_currMaterial = nullptr;
        m_currTexAlbedo = nullptr;
        m_currTexAmr = nullptr;
        m_currTexGlow = nullptr;
        
        [encoder setRenderPipelineState: m_drawPipeline ];
        [encoder setDepthStencilState: m_depthState];
        [encoder setFrontFacingWinding: MTLWindingClockwise];
        [encoder setCullMode: MTLCullModeBack];
        
        // Set the scene constant buffer for the pipeline
        Draw3dSceneConstants sceneConst;
        memcpy(&sceneConst.matProj, &scene->m_matProj, sizeof(Matrix4));
        memcpy(&sceneConst.matView, &scene->m_matView, sizeof(Matrix4));
        
        [encoder setVertexBytes:&sceneConst
                         length:sizeof(sceneConst)
                        atIndex:Draw3dBuffer_SceneConstants];
        
        Draw3dFragmentScene sceneFragment;
        memcpy(&sceneFragment.cameraPos, &scene->m_cameraPos, sizeof(Vector4));
        memcpy(&sceneFragment.globalLightDir, &scene->m_globalLightDir, sizeof(Vector4));
        memcpy(&sceneFragment.globalLightColour, &scene->m_globalLightColour, sizeof(Vector4));
        
        [encoder setFragmentBytes:&sceneFragment
                         length:sizeof(sceneFragment)
                        atIndex:Draw3dFragmentBuffer_Scene];
        

        for( CmdDrawModel * drawCmd = scene->m_modelDraw.m_head; drawCmd != nullptr; drawCmd = drawCmd->m_next ) {
            
            ModelInstance::Mesh * meshInst = drawCmd->m_modelInst->GetMeshAt(0);
            ModelMetal * model = (ModelMetal *) drawCmd->m_modelInst->GetModel();
            
            [encoder setVertexBuffer:model->m_vertices
                              offset:0
                             atIndex:0];
            
            [encoder setVertexBytes:&drawCmd->m_transform
                             length:sizeof(Matrix4)
                            atIndex:Draw3dBuffer_ModelConstants];
            
            for ( uint32_t m = 0; m < drawCmd->m_modelInst->GetMeshCount(); ++m) {
                
                if (meshInst->m_visible == 1) {
                
                    if ( meshInst->m_material != m_currMaterial ) {
                        UseMaterial( encoder, (MaterialLocal *) meshInst->m_material );
                        m_currMaterial = (MaterialLocal *) meshInst->m_material;
                    }
                    
                    [encoder setVertexBufferOffset: meshInst->m_mesh->m_vertexStart * 32
                                           atIndex: 0 ];
                
                    [encoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                                        indexCount: meshInst->m_mesh->m_indexCount
                                         indexType: MTLIndexTypeUInt32
                                       indexBuffer: model->m_indices
                                 indexBufferOffset: meshInst->m_mesh->m_indexStart * sizeof (uint32_t)];
                }
                
                ++meshInst;
            }
        
        }
        
#endif
    }
    
#define MATERIAL_TEXTURE_SAFE(tex, def) ((tex) != nullptr) ? (TextureBase*) (tex) : (TextureBase*) m_defaultTextures[def]
    
    //======================================================================================================================
    void RenderLocal::UseMaterial( i3d::Material * mat ) {
#if 1
        TextureBase * tex = MATERIAL_TEXTURE_SAFE( mat->m_albedo, DEFAULT_TEXTURE_RED );
        gfx::SetPixelTexture( tex->m_texture, 0 );
        
        tex = MATERIAL_TEXTURE_SAFE( mat->m_amr, DEFAULT_TEXTURE_AMR);
        gfx::SetPixelTexture( tex->m_texture, 1 );
        
        tex = MATERIAL_TEXTURE_SAFE( mat->m_glow, DEFAULT_TEXTURE_BLACK);
        gfx::SetPixelTexture( tex->m_texture, 2 );
#endif
    }
}

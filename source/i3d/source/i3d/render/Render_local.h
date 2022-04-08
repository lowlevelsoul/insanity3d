//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RENDER_METAL_H__
#define __I3D_RENDER_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxContext.h"
#include "i3d/render/Render.h"

#include "i3d/gfx/Gfx.h"
#include "i3d/render/DebugLineDraw.h"
#include "i3d/render/RenderBatch.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/render/RenderBuffers.h"
#include "i3d/render/StaticTransform.h"
#include "i3d/render/DrawLit.h"

namespace i3d {
    
    struct CmdScene3d;
    class MaterialLocal;
    class TextureMetal;
    class ModelMetal;
    class ModelInstance;
    
    class RenderLocal : public Render {
    public:
        RenderLocal();
        
        virtual ~RenderLocal();
        
        virtual void Initialise( RenderParams * params )override;

        virtual void Finalise()override;

        virtual void BeginScene()override;

        virtual void EndScene()override;

        virtual void BeginScene3d( const Matrix4& matProj, const Matrix4& matView, int32_t * viewport )override;

        virtual void EndScene3d()override;

        virtual void SubmitModel( Model * model, const Matrix4& transform, Material *  material )override;

        virtual void SubmitGlobalLight( const Vector3& dir, const Vector4& colour )override;

        virtual void SubmitDirectionLight( const Vector3& pos, const Vector3& dir, const Vector4& colour )override;

        virtual void SubmitPointLight( const Vector3& pos, float radius, float fallOff, const Vector4& colour )override;
        
        virtual void DrawLine( const Vector3 & v0, const Vector3 & v1, const Vector4 & colour ) override;
        
        virtual void DrawClosedLineList( const Vector3 * verts, size_t vertexCount, const Vector4 & colour ) override;

        virtual void Submit()override;
        
        bool SubmitBeginScene();
        
        void DoDraw( CmdScene * scene );
        
        void DoDraw3dOpaque( CmdScene3d * scene );
        
        void DoDrawBindless( CmdScene * scene );
        
        void DoDraw3dOpaqueBindless( CmdScene * scene );
        
        void CreateDefaultTextures();
        
        handle_t CreateAmrTexture( float ao, float metallic, float roughness );
        
        void UseMaterial( i3d::Material * mat );
        
        void BuildTexturePoolForFrame();
        
        void BuildLineBuffers( CmdScene * scene );
        
        void DoLines(  CmdScene3d * scene );
        
        void BuildSceneConstants( CmdScene * scene );
        
    public:
        bool                            m_useTextureHeap;
        bool                            m_useBindlessTextures;
        bool                            m_useMaterialBatching;
        
        RenderBatch                     m_batcher;
        CmdScene *                      m_currScene;
        
        gfx::Context *                  m_gfxContext;
        gfx::Pass *                     m_staticTransformPass;
        gfx::Pass *                     m_skinTransformPass;
        gfx::Pass *                     m_drawTransformPass;
        gfx::GpuTextureArray *          m_gpuTextureArray;
        
        Texture *                       m_defaultTextures[ DEFAULT_TEXTURE_COUNT ];
        
        //TextureArray                    m_textureArray;
        DrawLit                         m_drawLit;
        RenderBuffers                   m_renderBuffers;
        StaticTransform                 m_staticTransform;
        //SkinTransform                   m_skinTransform;
        DebugLineDraw                   m_lineDraw;
    };
    
    extern RenderLocal * renderLocal;
}

#endif

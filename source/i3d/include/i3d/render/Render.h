//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RENDER_H__
#define __I3D_RENDER_H__

#include "i3d/core/Types.h"
#include "i3d/math/Math3dTypes.h"

namespace i3d {
    
    class Model;
    class Material;
    class ModelInstance;
    
    class RenderParams {
    public:
        size_t      m_dbgLineMax;
        void *      m_nativeView;
        
        RenderParams() {
            m_nativeView = nullptr;
            m_dbgLineMax = 4096;
        }
        ~RenderParams() {}
    };
    
    class Render {
    public:
        enum DEFAULT_TEXTURE {
            DEFAULT_TEXTURE_BLACK=0,
            DEFAULT_TEXTURE_WHITE,
            DEFAULT_TEXTURE_GREY,
            DEFAULT_TEXTURE_RED,
            DEFAULT_TEXTURE_AMR,
            DEFAULT_TEXTURE_COUNT,
            DEFAULT_TEXTURE_FORCE32 = 0xffffffff,
        };
        
        virtual ~Render() {}
        
        virtual void Initialise( RenderParams * params ) = 0;

        virtual void Finalise() = 0;

        virtual void BeginScene() = 0;

        virtual void EndScene() = 0;

        virtual void BeginScene3d( const Matrix4& matProj, const Matrix4& matView, int32_t * viewport ) = 0;

        virtual void EndScene3d() = 0;

        virtual void SubmitModel( Model * model, const Matrix4& transform, Material *  material ) = 0;

        virtual void SubmitGlobalLight( const Vector3& dir, const Vector4& colour ) = 0;

        virtual void SubmitDirectionLight( const Vector3& pos, const Vector3& dir, const Vector4& colour ) = 0;

        virtual void SubmitPointLight( const Vector3& pos, float radius, float fallOff, const Vector4& colour ) = 0;
        
        virtual void DrawLine( const Vector3 & v0, const Vector3 & v1, const Vector4 & colour ) = 0;

        virtual void Submit() = 0;
    };
}

extern i3d::Render * render;


#endif

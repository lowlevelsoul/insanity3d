//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RENDERBATCH_H__
#define __I3D_RENDERBATCH_H__

#include "i3d/mem/HeapFrame.h"
#include "i3d/render/RenderBatchCmd.h"
#include "i3d/render/Material.h"

namespace i3d {
    class Model;
    class Material;
    class ModelInstance;
}

namespace i3d {
    
    class RenderBatch {
    public:
        template<typename __type__>
        __type__ * Alloc() {
            return (__type__*) m_heap->Alloc(sizeof(__type__));
        }
        
        template<typename __type__>
        __type__ * Alloc(size_t count) {
            size_t totalSize = sizeof(__type__) * count;
            return (__type__*) m_heap->Alloc( totalSize );
        }
        
        RenderBatch();
        
        ~RenderBatch();
        
        void Initialise( size_t memorySize, bool materialBatch );
        
        CmdScene * BeginScene();

        void EndScene();

        void BeginScene3d( const Matrix4& matProj, const Matrix4& matView, int32_t * viewport );

        void EndScene3d( );

        void SubmitModel( Model * model, const Matrix4& transform, Material * material );
        
        void DrawLine( const Vector3 & v0, const Vector3 & v1, const Vector4 & colour );
        
        CmdMaterial * GetMaterialCmd( Material * mat );
        
    public:
        HeapFrame *     m_heap;
        uint8_t *       m_memory;
        size_t          m_memorySize;
        
        size_t          m_materialCapacity;
        uint64_t        m_materialTimestamp;
        size_t          m_lineCapacity;
        
        CmdScene *      m_currScene;
        CmdScene3d *    m_currScene3d;
    };
    
    //======================================================================================================================
    inline CmdMaterial * RenderBatch::GetMaterialCmd( Material * mat ) {
        if ( mat->m_batchTimeStamp == m_materialTimestamp ) {
            return &m_currScene->m_materials[ mat->m_batchIndex ];
        }
        
        XE_ASSERT( m_currScene->m_materialCount < m_currScene->m_materialCapacity );
        
        
        
        CmdMaterial * matCmd = &m_currScene->m_materials[ m_currScene->m_materialCount ];
        matCmd->m_material = mat;
        matCmd->m_drawIndexStart = 0;
        matCmd->m_drawIndexCount = 0;

        mat->m_batchTimeStamp = m_materialTimestamp;
        mat->m_batchIndex = m_currScene->m_materialCount;

        m_currScene3d->m_materialListCount++;
        ++m_currScene->m_materialCount;
        
        return matCmd;        
    }
}

#endif

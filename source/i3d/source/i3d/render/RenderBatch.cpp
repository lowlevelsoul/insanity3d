//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/math/Math3d.h"
#include "i3d/render/Model.h"
#include "i3d/render/RenderBatch.h"
#include "i3d/render/Material.h"
#include "i3d/render/Model_local.h"

namespace i3d {
    
    //======================================================================================================================
    RenderBatch::RenderBatch() {
        m_memory = nullptr;
        m_memorySize = 0;
        m_heap = nullptr;
        
        m_currScene = nullptr;
        m_currScene3d = nullptr;
        
        m_materialCapacity = 256;
        m_materialTimestamp = 0;
        m_lineCapacity = 2048;
    }
    
    //======================================================================================================================
    RenderBatch::~RenderBatch() {
        
    }

    //======================================================================================================================
    void RenderBatch::Initialise( size_t memorySize, bool materialBatch ) {
        m_memory = new uint8_t[ memorySize ];
        m_memorySize = memorySize;
        m_heap = i3d::HeapFrame::Create( (uintptr_t) m_memory, m_memorySize );
        
        m_currScene = nullptr;
        m_currScene3d = nullptr;
    }
    
    //======================================================================================================================
    CmdScene * RenderBatch::BeginScene() {
        m_heap->Reset();
        
        m_currScene = Alloc<CmdScene>();
        m_currScene->m_scene3dList.Reset();
        
        m_currScene->m_materials  = Alloc<CmdMaterial>( m_materialCapacity );
        m_currScene->m_materialCapacity = (uint32_t) m_materialCapacity;
        m_currScene->m_materialCount = 0;
        m_currScene->m_transformedVertexCount = 0;
        
        ++m_materialTimestamp;
        
        return m_currScene;
    }

    //======================================================================================================================
    void RenderBatch::EndScene() {
        m_currScene = nullptr;
    }

    //======================================================================================================================
    void RenderBatch::BeginScene3d( const Matrix4& matProj, const Matrix4& matView, int32_t * viewport ) {
        
        m_currScene3d = Alloc<CmdScene3d>();
        
        m_currScene3d->m_matProj = matProj;
        m_currScene3d->m_matView.Inverse(matView);
        m_currScene3d->m_viewport[0] = viewport[0];
        m_currScene3d->m_viewport[1] = viewport[1];
        m_currScene3d->m_viewport[2] = viewport[2];
        m_currScene3d->m_viewport[3] = viewport[3];
        m_currScene3d->m_modelDraw.Reset();
        m_currScene3d->m_skins.Reset();
        
        Vector3 dir(0, -20, 20);
        dir.Normalise();
        m_currScene3d->m_globalLightDir.Set( dir, 1 );
        m_currScene3d->m_globalLightColour.Set(2, 2, 2, 1);
        m_currScene3d->m_cameraPos = matView.m_rows[3];
        
        m_currScene3d->m_lineVerts = nullptr;
        m_currScene3d->m_lineIndices = nullptr;
        m_currScene3d->m_lineIndexCapacity = 0;
        m_currScene3d->m_lineIndexCount = 0;
        m_currScene3d->m_lineVertexCapacity = 0;
        m_currScene3d->m_lineVertexCount = 0;
        m_currScene3d->m_materialListStart = m_currScene->m_materialCount;
        m_currScene3d->m_materialListCount = 0;

    
        m_currScene->m_scene3dList.Add( m_currScene3d );
    }

    //======================================================================================================================
    void RenderBatch::EndScene3d( ) {
        m_currScene3d = nullptr;
    }
    
    //======================================================================================================================
    void RenderBatch::DrawLine( const Vector3 & v0, const Vector3 & v1, const Vector4 & colour ) {
        
        if ( m_currScene3d->m_lineVertexCapacity == 0 ) {
            m_currScene3d->m_lineVertexCapacity = m_lineCapacity * 2;
            m_currScene3d->m_lineIndexCapacity = m_lineCapacity * 2;
            m_currScene3d->m_lineVerts = Alloc<CmdLineVertex>( m_currScene3d->m_lineVertexCapacity );
            m_currScene3d->m_lineIndices = Alloc<uint32_t>( m_currScene3d->m_lineVertexCapacity );
        }
        
        if ( m_currScene3d->m_lineIndexCount >= m_currScene3d->m_lineIndexCapacity - 1 ||
             m_currScene3d->m_lineVertexCount >= m_currScene3d->m_lineVertexCapacity - 1 ) {
            return;
        }
        
        CmdLineVertex * v = &m_currScene3d->m_lineVerts[ m_currScene3d->m_lineVertexCount ];
        uint32_t * i = &m_currScene3d->m_lineIndices[ m_currScene3d->m_lineIndexCount ];
        
        v[0].m_pos = Vector4(v0,1);
        v[0].m_colour = colour;
        
        v[1].m_pos = Vector4(v1, 1);
        v[1].m_colour = colour;
        
        i[0] = (uint32_t) m_currScene3d->m_lineVertexCount;
        i[1] = (uint32_t) m_currScene3d->m_lineVertexCount+1;
        
        m_currScene3d->m_lineVertexCount += 2;
        m_currScene3d->m_lineIndexCount += 2;
    }

    //======================================================================================================================
    void RenderBatch::SubmitModel( Model * model, const Matrix4& transform, Material * mat ) {
        CmdDrawModel * drawCmd = Alloc<CmdDrawModel>();
        
        CmdMaterial * matCmd = GetMaterialCmd( mat );
        matCmd->m_drawList.Add( drawCmd );
        matCmd->m_drawIndexCount += model->GetIndexCount();
        
        drawCmd->m_model        = model;
        drawCmd->m_transform    = transform;
        drawCmd->m_transformVertexStart = m_currScene->m_transformedVertexCount;

        m_currScene->m_transformedVertexCount += model->GetVertexCount();
    }
}

//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "ToolMeshSmoother.h"
#include "ToolMesh.h"



    //==========================================================================================================================================
    ToolMeshSmoother::ToolMeshSmoother() {
        
    }

    //==========================================================================================================================================
    ToolMeshSmoother::~ToolMeshSmoother() {
        
    }

    //==========================================================================================================================================
    void ToolMeshSmoother::Process( ToolMesh * mesh) {
        for ( uint32_t i = 0; i < mesh->m_triangleIndices.size(); i+=3 ) {
            Triangle tri;
            
            tri.v0 = m_verts.AddItem( mesh->m_verts[ mesh->m_triangleIndices[i] ] );
            tri.v1 = m_verts.AddItem( mesh->m_verts[ mesh->m_triangleIndices[i+1] ] );
            tri.v2 = m_verts.AddItem( mesh->m_verts[ mesh->m_triangleIndices[i+2] ] );
            
            m_vertexTris.push_back(tri);
        }
    }
}

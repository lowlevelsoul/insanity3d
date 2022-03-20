//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __XENGINE_TOOLMESHSMOOTHER_H__
#define __XENGINE_TOOLMESHSMOOTHER_H__

#include "xe/core/Types.h"
#include "xe/util/toolmodel/ToolUniqueList.h"

namespace xe {

    class ToolMesh;

    class ToolMeshSmoother {
    public:
        struct Triangle {
            uint32_t v0, v1, v2;
            typedef std::vector<Triangle> vector_t;
        };
        
        ToolMeshSmoother();
        
        ~ToolMeshSmoother();
        
        void Process( ToolMesh * mesh);
        
    protected:
        ToolUniqueList<xe::Vector3>     m_verts;
        ToolUniqueList<xe::Vector3>     m_normals;
        ToolUniqueList<xe::Vector3>     m_texCoords;
        
        Triangle::vector_t          m_vertexTris;
        Triangle::vector_t          m_normalTris;
        Triangle::vector_t          m_texCoordTris;
    };
}

#endif

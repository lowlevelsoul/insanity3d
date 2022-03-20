//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLSKIN_H__
#define __TOOLSKIN_H__

#include "i3d/util/RefObject.h"
#include "i3d/util/RefPointer.h"

namespace i3d {

    class ToolMesh;
    class ToolSkin;

    class ToolSkinCluster  : public RefObject {
    public:
        typedef RefPointer<ToolSkinCluster> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
        
        ToolSkinCluster( ToolSkin * skin );
        
        virtual ~ToolSkinCluster();

         void Copy( ToolSkinCluster * rhs );
        
    public:
        ToolSkin *                  m_skin;             ///< Pointer to the skin that this cluster belongs to
        std::string                 m_joint;            ///< The name of the joint this clusteris associated with
        std::vector<uint32_t>       m_verts;            ///< List of vertex indices that this cluster affects
        std::vector<float>          m_weights;          ///< List of weights
        i3d::Matrix4                m_offset;           ///< Offset matrix used to transform the vertices for the skin
    };

    class ToolSkin : public RefObject {
    public:
        typedef RefPointer<ToolSkin> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
        
        ToolSkin( ToolMesh * mesh );
        
        virtual ~ToolSkin();

        ToolSkinCluster * AddCluster();

        void Copy( ToolSkin * rhs );
        
    public:
        ToolMesh *                  m_mesh;             ///< The mesh that this skin is attached to
        ToolSkinCluster::vector_t   m_clusters;         ///< Array of clusters for the skin
    };
}

#endif

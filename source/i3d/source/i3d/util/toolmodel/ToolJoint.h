//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLJOINT_H__
#define __TOOLJOINT_H__

#include "i3d/util/RefObject.h"
#include "i3d/util/RefPointer.h"
#include "i3d/util/toolmodel/ToolMesh.h"

namespace i3d {

    class ToolSkeleton;

    class ToolJoint : public RefObject {
    public:
        typedef RefPointer<ToolJoint> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
        
        ToolJoint();
        
        virtual ~ToolJoint();

        bool HasMeshes() const {
            return m_meshes.empty() == false;
        }
        
        i3d::Matrix4 EvaluateGlobal();
        
    public:
        std::string             m_name;             ///< Joint name
        ToolSkeleton *          m_skeleton;         ///< Pointer to the skeleton that this joint belongs to
        i3d::Matrix4                 m_transform;        ///< Transform of this joint
        i3d::Matrix4                 m_globalTransform;  ///< Global transform
        ToolJoint *             m_parent;           ///< Pointer to the parent joint
        ToolJoint::vector_t     m_children;         ///< List of child joints
        ToolMesh::vector_t      m_meshes;           ///< Meshes that are attached to this joint
    };
}

#endif

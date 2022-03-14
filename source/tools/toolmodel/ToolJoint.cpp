//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "toolmodel/ToolJoint.h"

//==========================================================================================================================================
ToolJoint::ToolJoint() {
    m_transform = i3d::Matrix4::IDENTITY;
    m_globalTransform = i3d::Matrix4::IDENTITY;
    m_parent = nullptr;
    m_skeleton = nullptr;
}

//==========================================================================================================================================
ToolJoint::~ToolJoint() {
    
}

//==========================================================================================================================================
i3d::Matrix4 ToolJoint::EvaluateGlobal() {
    if (m_parent != nullptr) {
        return m_transform;
    }

    i3d::Matrix4 parent = m_parent->EvaluateGlobal();
    i3d::Matrix4 global = m_transform * parent;
    
    return global;
}

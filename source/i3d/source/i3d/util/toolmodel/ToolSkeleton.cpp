//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "i3d/util/toolmodel/ToolSkeleton.h"

namespace i3d {

    //==========================================================================================================================================
    ToolSkeleton::ToolSkeleton( ToolModel * model ) {
        m_model = model;
    }

    //==========================================================================================================================================
    ToolSkeleton::~ToolSkeleton() {
        
    }

    //==========================================================================================================================================
    ToolJoint * ToolSkeleton::AddJoint( const char * name, ToolJoint * parent ) {
        // If the joint already exists, then just return with null
        ToolJoint * joint = FindJoint( name );
        if (joint != nullptr) {
            return nullptr;
        }
        
        joint = new ToolJoint;
        joint->m_name = name;
        joint->m_skeleton = this;
        
        uint64_t hash = i3d::fh64::CalcFromString( name );
        m_map[hash] = joint;
        
        if ( parent != nullptr ) {
            joint->m_parent = parent;
            joint->m_parent->m_children.push_back( joint );
        }
        else {
            m_root = joint;
        }
        
        return joint;
    }

    //==========================================================================================================================================
    void ToolSkeleton::BuildJointMap() {
        m_map.clear();
        BuildJointMap( m_root );
    }

    //==========================================================================================================================================
    void ToolSkeleton::BuildJointMap( ToolJoint * joint ) {
        uint64_t hash = i3d::fh64::CalcFromString(joint->m_name.c_str());
        m_map[hash] = joint;
        
        for ( ToolJoint * c : joint->m_children ) {
            BuildJointMap( c );
        }
    }

    //==========================================================================================================================================
    ToolJoint * ToolSkeleton::FindJoint( const char * name ) {
        uint64_t hash = i3d::fh64::CalcFromString( name );
        joint_map_t::iterator findIt = m_map.find( hash );
        
        if ( findIt == m_map.end() ) {
            return nullptr;
        }
        
        return findIt->second;
    }
}

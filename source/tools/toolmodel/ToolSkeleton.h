//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLSKELETON_H__
#define __TOOLSKELETON_H__

#include "i3d/core/Types.h"
#include "toolapp/RefObject.h"
#include "toolapp/RefPointer.h"
#include "toolmodel/ToolJoint.h"

class ToolModel;

class ToolSkeleton : public RefObject {
public:
    typedef RefPointer<ToolSkeleton> ref_ptr_t;
    typedef std::vector<ToolSkeleton::ref_ptr_t> vector_t;
    
    ToolSkeleton( ToolModel * model );
    
    virtual ~ToolSkeleton();
    
    void BuildJointMap();
    
    void BuildJointMap( ToolJoint * joint );
    
    ToolJoint * AddJoint( const char * name, ToolJoint * parent );
    
    ToolJoint * FindJoint( const char * name );
    
public:
    typedef std::map<uint64_t, ToolJoint*> joint_map_t;
    
    ToolModel *             m_model;
    ToolJoint::ref_ptr_t    m_root;         ///< The root joint
    joint_map_t             m_map;          ///< Map used to seach for skeleton joints by name
};

#endif

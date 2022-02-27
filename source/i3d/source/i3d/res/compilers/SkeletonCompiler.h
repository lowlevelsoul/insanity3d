//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_SKELETONCOMPILER_H__
#define __XENGINE_SKELETONCOMPILER_H__

#include "xe/util/toolmodel/ToolJoint.h"
#include "xe/util/toolmodel/ToolSkeleton.h"
#include "xe/util/ToolMemStream.h"
#include "xe/render/SkeletonStream.h"
#include "xe/res/ResourceCompiler.h"

namespace xe {

    class SkeletonCompiler : public xe::ResourceCompiler {
    public:
        RTTI_CLASS_DECLARE( SkeletonCompiler, ResourceCompiler )
        
        SkeletonCompiler();

        ~SkeletonCompiler();
        
        void Write( ToolMemStream & str, ToolSkeleton * skel );
        
        void WriteHeader( ToolMemStream & str,  xe::SkeletonStream & header );

        void BuildJointList( ToolSkeleton * skeleton, bool ignoreAttachments = true );

        void AddJoint( ToolJoint * joint, bool ignoreAttachments );

        int32_t FindJoint( const char * name );
        
        void WriteJointNames();
        
        void WriteJointInfo();
        
        uint32_t WriteString( const char * name );
        
    public:
        typedef std::map<uint64_t, int32_t> joint_map_t;
        
        float       m_scale;
        xe::String  m_rootJointName;
        bool        m_pruneMixamoMarkers;
        bool        m_pruneRootNode;
        
        ToolJoint::vector_t             m_joints;
        std::vector<xe::Matrix4>        m_localMats;
        std::vector<xe::Matrix4>        m_globalMats;
        std::vector<xe::Matrix4>        m_globalInvMats;
        joint_map_t                     m_jointMap;
        
        ToolMemStream               m_jointParentIndices;
        ToolMemStream               m_jointMatrices;
        ToolMemStream               m_jointBindMatrices;
        ToolMemStream               m_jointHashArray;
        ToolMemStream               m_jointHashMap;
        ToolMemStream               m_jointStrings;
        ToolMemStream               m_jointNames;
    };
}

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SKELETONCOMPILER_H__
#define __I3D_SKELETONCOMPILER_H__

#include "i3d/util/toolmodel/ToolJoint.h"
#include "i3d/util/toolmodel/ToolSkeleton.h"
#include "i3d/util/ToolMemStream.h"
#include "i3d/render/SkeletonStream.h"
#include "i3d/res/ResourceBuilder.h"

namespace i3d {

    class SkeletonBuilder : public ResourceBuilder {
    public:
        RTTI_CLASS_DECLARE( SkeletonBuilder, ResourceBuilder )
        
        SkeletonBuilder();

        ~SkeletonBuilder();
        
        void Write( ToolMemStream & str, ToolSkeleton * skel );
        
        void WriteHeader( ToolMemStream & str,  SkeletonStream & header );

        void BuildJointList( ToolSkeleton * skeleton, bool ignoreAttachments = true );

        void AddJoint( ToolJoint * joint, bool ignoreAttachments );

        int32_t FindJoint( const char * name );
        
        void WriteJointNames();
        
        void WriteJointInfo();
        
        uint32_t WriteString( const char * name );
        
    public:
        typedef std::map<uint64_t, int32_t> joint_map_t;
        
        float               m_scale;
        stl::String::type   m_rootJointName;
        bool                m_pruneMixamoMarkers;
        bool                m_pruneRootNode;
        
        ToolJoint::vector_t             m_joints;
        std::vector<Matrix4>            m_localMats;
        std::vector<Matrix4>            m_globalMats;
        std::vector<Matrix4>            m_globalInvMats;
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

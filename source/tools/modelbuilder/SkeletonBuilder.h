//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SKELETONBUILDER_H__
#define __SKELETONBUILDER_H__

#include "toolmodel/ToolJoint.h"
#include "toolmodel/ToolSkeleton.h"
#include "i3d/render/SkeletonStream.h"

class SkeletonBuilder {
public:
    class Options {
    public:
        Options() {
            m_scale = 1;
            m_pruneMixamoMarkers = false;
            m_pruneRootNode = false;
        }
        
        float       m_scale;
        std::string m_rootJointName;
        bool        m_pruneMixamoMarkers;
        bool        m_pruneRootNode;
        
        static const Options DEFAULT;
    };
    
    SkeletonBuilder( const Options & options = Options::DEFAULT );

	~SkeletonBuilder();
    
    void Write( ToolMemStream & str, ToolSkeleton * skel );
    
    void WriteHeader( ToolMemStream & str,  i3d::SkeletonStream & header );

    void BuildJointList( ToolSkeleton * skeleton, bool ignoreAttachments = true );

	void AddJoint( ToolJoint * joint, bool ignoreAttachments );

	int32_t FindJoint( const char * name );
    
    void WriteJointNames();
    
    void WriteJointInfo();
    
    uint32_t WriteString( const char * name );
    
public:
    typedef std::map<uint64_t, int32_t> joint_map_t;
    
    Options                     m_options;
	ToolJoint::vector_t		    m_joints;
    std::vector<i3d::Matrix4>   m_localMats;
	std::vector<i3d::Matrix4>	m_globalMats;
	std::vector<i3d::Matrix4>	m_globalInvMats;
	joint_map_t				    m_jointMap;
    
    ToolMemStream               m_jointParentIndices;
    ToolMemStream               m_jointMatrices;
    ToolMemStream               m_jointBindMatrices;
    ToolMemStream               m_jointHashArray;
    ToolMemStream               m_jointHashMap;
    ToolMemStream               m_jointStrings;
    ToolMemStream               m_jointNames;
};

#endif

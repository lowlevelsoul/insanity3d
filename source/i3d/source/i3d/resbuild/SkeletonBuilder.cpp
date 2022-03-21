//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/resbuild/SkeletonBuilder.h"

namespace i3d {
    RTTI_CLASS_BEGIN( SkeletonBuilder )
        RTTI_PROP( FLOAT,       "scale",                    m_scale )
        RTTI_PROP( STRING,      "string",                   m_rootJointName )
        RTTI_PROP( BOOL,        "prune_mmixamo_markers",    m_pruneMixamoMarkers )
        RTTI_PROP( BOOL,        "prune_root_node",          m_pruneRootNode )
    RTTI_CLASS_END( SkeletonBuilder )

    //======================================================================================================================
    SkeletonBuilder::SkeletonBuilder() {
        m_scale = 1;
        m_pruneRootNode = false;
        m_pruneMixamoMarkers = false;
    }

    //======================================================================================================================
    SkeletonBuilder::~SkeletonBuilder() {

    }

    //======================================================================================================================
    void SkeletonBuilder::Write( ToolMemStream & str, ToolSkeleton * skel ) {
        // Build the list of joints and calculate the local, global and
        // inverse global matrices
        BuildJointList( skel );
        
        // Build the various streams containing the info about the skeleton
        WriteJointNames();
        WriteJointInfo();
        
        SkeletonStream header;
        memset( &header, 0, sizeof(header) );
        header.m_version = SkeletonStream::VERSION;
        header.m_jointCount = (uint32_t) m_joints.size();
        
        uint64_t headerPos = str.Tell();
        WriteHeader( str, header );
        
        header.m_offsJointParents = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointParentIndices );
        
        header.m_offsJointHashArray = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointHashArray );
        
        header.m_offsJointHashMap = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointHashMap );
        
        header.m_offsJointMatrices = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointMatrices );
        
        header.m_offsJointBindMatrices = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointBindMatrices );
        
        header.m_offsNameStrings = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointStrings );
        
        header.m_offsNames = (uint32_t) ( str.Tell() - headerPos );
        str.Write( m_jointNames );
        
        uint64_t endPos = str.Tell();
        str.Seek( headerPos );
        WriteHeader( str, header );
        str.Seek( endPos );
    }

    //======================================================================================================================
    void SkeletonBuilder::BuildJointList( ToolSkeleton * skeleton, bool ignoreAttachments ) {
        // Recursively add the joints
        if ( m_rootJointName.empty() == true) {
            AddJoint( skeleton->m_root, ignoreAttachments );
        }
        else {
            ToolJoint * rootJoint = skeleton->FindJoint( m_rootJointName.c_str() );
            XE_ERROR(rootJoint == nullptr, "Root joint '%s' was not found in skeleton",  m_rootJointName.c_str() );
        }

        // Caclulate global matrices
        for ( auto j : m_joints ) {
            if ( j->m_parent == nullptr ) {
                
                Matrix4 mat = j->m_transform;
                mat.m_rows[3] = Vector4( Vector3( mat.m_rows[3] ) * m_scale, 1);
                m_globalMats.push_back( j->m_transform );
                m_localMats.push_back( mat );
            }
            else {
                int32_t parentIndex = FindJoint( j->m_parent->m_name.c_str() );
                XE_ERROR( parentIndex < 0, "Could not find parent joint whilst buidling skeleton\n" );
                
                Matrix4 mat = j->m_transform;
                mat.m_rows[3] = Vector4( Vector3( mat.m_rows[3] ) * m_scale, 1);
                m_localMats.push_back( mat );
                
                Matrix4 global =  mat * m_globalMats[ parentIndex ];
                m_globalMats.push_back( global );
            }
        }

        // Calculate inverse global mats
        for ( auto & m : m_globalMats ) {
            Matrix4 i;
            i.Inverse( m );
            m_globalInvMats.push_back( i );
        }
    }

    //======================================================================================================================
    void SkeletonBuilder::AddJoint( ToolJoint * joint, bool ignoreAttachments ) {
        
        bool addJoint = true;
        
        if (ignoreAttachments == true) {
            addJoint = joint->HasMeshes() == false;
        }
        
        if ( m_pruneMixamoMarkers == true && addJoint == true ) {
            bool isMarker = ( ( joint->m_name.find(".R_end") != std::string::npos ) || ( joint->m_name.find(".L_end") != std::string::npos ) );
            addJoint = ( isMarker == false );
        }

        if (  addJoint ) {

            int32_t index = ( int32_t ) m_joints.size();
            uint64_t hash = fh64::CalcFromString( joint->m_name.c_str() );
            
            // Add a copy of the joint
            ToolJoint * copyJoint = new ToolJoint;
            copyJoint->m_name = joint->m_name;
            copyJoint->m_transform = joint->m_transform;
            
            // Setup parent info for the copied joint
            if ( joint->m_parent != nullptr ) {
                int32_t parentIndex = FindJoint( joint->m_parent->m_name.c_str() );
                ToolJoint * parent = m_joints[ parentIndex ];
                
                parent->m_children.push_back( copyJoint );
                copyJoint->m_parent = parent;
            }
            
            m_jointMap[ hash ] = index;
            m_joints.push_back( copyJoint );

            for ( auto c : joint->m_children ) {
                AddJoint( c, ignoreAttachments );
            }
        }
    }

    //======================================================================================================================
    int32_t SkeletonBuilder::FindJoint( const char * name ) {
        uint64_t hash = fh64::CalcFromString( name );
        joint_map_t::iterator findIt = m_jointMap.find( hash );
        if ( findIt == m_jointMap.end() ) {
            return -1;
        }

        return findIt->second;
    }

    //======================================================================================================================
    void SkeletonBuilder::WriteJointInfo() {
        uint32_t jointIndex = 0;
        for( auto j : m_joints ) {
            
            int32_t parentIndex = ( j->m_parent == nullptr ) ? -1 : FindJoint( j->m_parent->m_name.c_str() );
            
            m_jointParentIndices.Write( &parentIndex, 1);
            m_jointMatrices.Write( (float *) &m_localMats[ jointIndex ], 16 );
            m_jointBindMatrices.Write( (float *) &m_globalInvMats[ jointIndex ], 16 );
            ++jointIndex;
        }
    }

    //======================================================================================================================
    void SkeletonBuilder::WriteJointNames( ) {
        
        std::vector<uint64_t> hashes;
        std::vector<int32_t> map;
        
        uint32_t jointIndex = 0;

        for ( auto j : m_joints ) {
            
            WriteString( j->m_name.c_str() );
            
            uint64_t hash = fh64::CalcFromString( j->m_name.c_str() );
            
            if ( hashes.empty() == true ) {
                hashes.push_back( hash );
                map.push_back( jointIndex );
            }
            else {
                int32_t index;
                Sys::BinarySearch<uint64_t, int32_t>( index, hash, &hashes[0], hashes.size() );
                
                hashes.insert( hashes.begin() + index, hash );
                map.insert( map.begin() + index, jointIndex );
            }
            
            ++jointIndex;
        }
        
        m_jointHashArray.Write( &hashes[0], hashes.size() );
        m_jointHashMap.Write( &map[0], map.size() );
    }

    //======================================================================================================================
    uint32_t SkeletonBuilder::WriteString( const char * name ) {
        uint32_t pos = (uint32_t) m_jointStrings.Length();
        
        size_t len = strlen(name);
        m_jointStrings.Write( name, len+1);
        
        m_jointNames.Write( &pos );
        return pos;
    }


    //======================================================================================================================
    void SkeletonBuilder::WriteHeader( ToolMemStream & str,  SkeletonStream & header ) {
        str.Write( &header.m_version );
        str.Write( &header.m_flags );
        str.Write( &header.m_jointCount );
        str.Write( &header.m_offsJointParents );
        str.Write( &header.m_offsJointHashArray );
        str.Write( &header.m_offsJointHashMap );
        str.Write( &header.m_offsJointMatrices );
        str.Write( &header.m_offsJointBindMatrices );
        str.Write( &header.m_offsNameStrings );
        str.Write( &header.m_offsNames );
    }
}

//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "toolmodel/ToolSkin.h"

//==========================================================================================================================================
ToolSkinCluster::ToolSkinCluster( ToolSkin * skin ) {
    m_skin = skin;
    m_offset = i3d::Matrix4::IDENTITY;
}

//==========================================================================================================================================
ToolSkinCluster::~ToolSkinCluster() {
    
}

//==========================================================================================================================================
void ToolSkinCluster::Copy( ToolSkinCluster * rhs ) {
    m_offset = rhs->m_offset;
    m_joint = rhs->m_joint;
    m_verts = rhs->m_verts;
    m_weights = rhs->m_weights;
}

//==========================================================================================================================================
ToolSkin::ToolSkin( ToolMesh * mesh ) {
    m_mesh = mesh;
}

//==========================================================================================================================================
ToolSkin::~ToolSkin() {
    
}

//==========================================================================================================================================
ToolSkinCluster * ToolSkin::AddCluster() {
    ToolSkinCluster * cluster = new ToolSkinCluster( this );
    m_clusters.push_back( cluster );
    return cluster;
}

//==========================================================================================================================================
void ToolSkin::Copy( ToolSkin * rhs ) {

    m_clusters.resize( 0 );
   
    for ( auto srcCluster : rhs->m_clusters ) {
        ToolSkinCluster * cluster = AddCluster();
        cluster->Copy( srcCluster );
    }
}

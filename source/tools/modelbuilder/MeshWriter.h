//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __MESHWRITER_H__
#define __MESHWRITER_H__

#include "i3d/render/ModelStream.h"
#include "toolmodel/ToolModel.h"

#if 0
class MeshWriter {
public:
    MeshWriter();
    
    ~MeshWriter();
    
    bool Write( ToolMemStream& stream, ToolMesh::vector& meshes, std::string& errorMsg );
    
protected:
    
    bool WriteMeshHeader( ToolMemStream& stream, const i3d::ModelStream& header );
    
    bool WriteVertices( ToolMemStream& stream, ToolMesh::vector& meshes );
    
    bool WriteIndices( ToolMemStream& stream, ToolMesh::vector& meshes );
    
    void CalcBounds( i3d::Vector3& bmin, i3d::Vector3& bMax, ToolMesh::vector& meshes );
};

#endif


#endif

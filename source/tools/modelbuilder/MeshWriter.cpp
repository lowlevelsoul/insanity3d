//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "modelbuilder/MeshWriter.h"
#include "toolmodel/ToolModel.h"
#include "toolmodel/ToolMesh.h"

#if 0

//======================================================================================================================
MeshWriter::MeshWriter() {
    
}

//======================================================================================================================
MeshWriter::~MeshWriter() {
    
}

//======================================================================================================================
bool MeshWriter::Write(ToolMemStream& stream, ToolMesh::vector& meshes, std::string& errorMsg) {
    
    uintptr_t offsHeader = stream.Tell();
    
    // Calc the bounds of all of meshes.
    i3d::Vector3 bmin, bmax;
    CalcBounds(bmin, bmax, meshes);
    
    // Fill out and mesh header with the info that we have right now,
    // and write it to the stream
    i3d::ModelStream header;
    memset(&header, 0, sizeof(header));
    
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    
    for(ToolMesh * mesh : meshes) {
        vertexCount += (uint32_t) mesh->m_verts.size();
        indexCount += (uint32_t) (mesh->m_tris.size() * 3);
    }
    
    header.m_flags          = 0;
    header.m_version        = i3d::ModelStream::VERSION;
    header.m_vertexCount    = vertexCount;
    header.m_indexCount     = indexCount;
    header.m_bounds[0][0]   = bmin.X();
    header.m_bounds[0][1]   = bmin.Y();
    header.m_bounds[0][2]   = bmin.Z();
    header.m_bounds[1][0]   = bmax.X();
    header.m_bounds[1][1]   = bmax.Y();
    header.m_bounds[1][2]   = bmax.Z();

    WriteMeshHeader(stream, header);
    
    i3d::Vector3 bsize = bmax - bmin;
    XE_LOG( "    Bounds Min : <%5.5f, %5.5f, %5.5f>\n", bmin.X(), bmin.Y(), bmin.Z());
    XE_LOG( "    Bounds Max : <%5.5f, %5.5f, %5.5f>\n", bmax.X(), bmax.Y(), bmax.Z());
    XE_LOG("     Dimensions : <%5.5f, %5.5f, %5.5f>\n", bsize.X(), bsize.Y(), bsize.Z());
    
    // Write out vertex and index data
    header.m_offsVerts = (uint32_t)(stream.Tell() - offsHeader);
    WriteVertices(stream, meshes);
    
    header.m_offsIndices = (uint32_t)(stream.Tell() - offsHeader);
    WriteIndices(stream, meshes);
    
    // Re-write the now completed mesh header
    uintptr_t offsEnd = stream.Tell();
    stream.Seek(offsHeader);
    WriteMeshHeader(stream, header);
    stream.Seek(offsEnd);
    
    return true;
}

//======================================================================================================================
bool MeshWriter::WriteMeshHeader(ToolMemStream& stream, const i3d::ModelStream& header) {
    stream.Write(&header.m_flags);
    stream.Write(&header.m_version);
    stream.Write(&header.m_vertexCount);
    stream.Write(&header.m_indexCount);
    stream.Write(&header.m_bounds[0][0], 3);
    stream.Write(&header.m_bounds[1][0], 3);
    stream.Write(&header.m_offsVerts);
    stream.Write(&header.m_offsIndices);
    
    return true;
}

//======================================================================================================================
bool MeshWriter::WriteVertices(ToolMemStream& stream, ToolMesh::vector& meshes) {
    
    for(auto mesh : meshes) {
        
        for(uint32_t v=0; v < mesh->m_verts.size(); ++v) {
            float pos4[4];
            float norm4[4];
            
            pos4[0] = mesh->m_verts[v].X();
            pos4[1] = mesh->m_verts[v].Y();
            pos4[2] = mesh->m_verts[v].Z();
            pos4[3] = mesh->m_texCoords[v].X();
            
            norm4[0] = mesh->m_normals[v].X();
            norm4[1] = mesh->m_normals[v].Y();
            norm4[2] = mesh->m_normals[v].Z();
            norm4[3] = mesh->m_texCoords[v].Y();
            
            stream.Write(pos4, 4);
            stream.Write(norm4, 4);
        }
    }
    
    return true;
}


//======================================================================================================================
bool MeshWriter::WriteIndices(ToolMemStream& stream, ToolMesh::vector& meshes) {
    
    uint32_t vertexOffset = 0;
    
    for(auto mesh : meshes) {
        
        for(auto t : mesh->m_tris) {
            uint32_t v[3];
            
            v[0] = vertexOffset + t.m_verts[0];
            v[1] = vertexOffset + t.m_verts[1];
            v[2] = vertexOffset + t.m_verts[2];
            
            stream.Write(v, 3);
        }
        
        vertexOffset += mesh->m_verts.size();
    }
    
    return true;
}

//======================================================================================================================
void MeshWriter::CalcBounds( i3d::Vector3& bmin, i3d::Vector3& bmax, ToolMesh::vector& meshes) {
    bmin = meshes[0]->m_boundsMin;
    bmax = meshes[0]->m_boundsMax;
    
    for(uint32_t i=1; i < meshes.size(); ++i) {
        bmin.Min(bmin, meshes[i]->m_boundsMin);
        bmax.Max(bmax, meshes[i]->m_boundsMax);
    }
}

#endif

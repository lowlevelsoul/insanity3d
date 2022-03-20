//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "ToolMesh.h"
#include "ToolSkeleton.h"
#include "ToolModel.h"
#include "ToolUniqueList.h"

namespace i3d {

    //==========================================================================================================================================
    ToolMesh::ToolMesh( ToolModel * model ) {
        m_model = model;
        m_boundsMin = i3d::Vector3::ZERO;
        m_boundsMax = i3d::Vector3::ZERO;
    }

    //==========================================================================================================================================
    ToolMesh::~ToolMesh() {
        
    }

    //==========================================================================================================================================
    void ToolMesh::CalcBounds() {
        m_boundsMin = m_verts[0];
        m_boundsMax = m_boundsMin;

        for ( auto & v : m_verts ) {
            m_boundsMin.Min( m_boundsMin, v );
            m_boundsMax.Max( m_boundsMax, v );
        }
    }

    //==========================================================================================================================================
    ToolSkin * ToolMesh::AddSkin() {
        ToolSkin * skin = new ToolSkin( this );
        m_skin = skin;
        return skin;
    }

    //==========================================================================================================================================
    bool ToolMesh::IsSkinned() const {
        return ( m_skin != nullptr );
    }

    //==========================================================================================================================================
    void ToolMesh::SetMaterial( const char * name ) {
        if ( name == nullptr ) {
            m_material = nullptr;
        }
        else {
            ToolMaterial * mat = m_model->FindMaterial( name );
            m_material = mat;
        }
    }

    //==========================================================================================================================================
    void ToolMesh::Copy( ToolMesh * rhs ) {
        m_name = rhs->m_name;
        m_verts = rhs->m_verts;
        m_normals = rhs->m_normals;
        m_texcoords = rhs->m_texcoords;
        m_triangleIndices = rhs->m_triangleIndices;

        if ( rhs->m_skin ) {
            AddSkin();
            m_skin->Copy( rhs->m_skin );
        }
    }

    //==========================================================================================================================================
    void ToolMesh::MakeSkinShell() {
        std::vector<i3d::Vector3> vertsAccum;
        std::vector<i3d::Vector3> normsAccum;

        vertsAccum.reserve( m_verts.size() );
        normsAccum.reserve( m_normals.size() );

        for ( uint32_t i = 0; i < m_verts.size(); ++i ) {
            vertsAccum.push_back( i3d::Vector3::ZERO );
            normsAccum.push_back( i3d::Vector3::ZERO );
        }

        for ( auto c : m_skin->m_clusters ) {

            for ( uint32_t v = 0; v < c->m_verts.size(); ++v ) {

                uint32_t vertIndex = c->m_verts[ v ];

                i3d::Vector4 sv = c->m_offset * i3d::Vector4( m_verts[ vertIndex ], 1 );
                i3d::Vector4 sn = c->m_offset * i3d::Vector4( m_normals[ vertIndex ], 0 );

                vertsAccum[ vertIndex ] += ( i3d::Vector3( sv.X(), sv.Y(), sv.Z() ) * c->m_weights[ v ] );
                normsAccum[ vertIndex ] += ( i3d::Vector3( sn.X(), sn.Y(), sn.Z() ) * c->m_weights[ v ] );
            }

            // Clear the offset matrix for this cluster, as we don't need it any more.
            c->m_offset = i3d::Matrix4::IDENTITY;
        }

        // Make sure our normals are all of unit length
        for ( auto & n : normsAccum ) {
            n.Normalise();
        }

        // Replace the vertex positions and normals
        m_verts = vertsAccum;
        m_normals = normsAccum;

        CalcBounds();
    }

    //==========================================================================================================================================
    void ToolMesh::TransformByJoint( ToolJoint * joint ) {
        i3d::Matrix4 globalTransform = joint->EvaluateGlobal();
        ApplyTransform( globalTransform );
    }

    //==========================================================================================================================================
    void ToolMesh::ApplyTransform( const i3d::Matrix4 & transform ) {
        
        std::vector<i3d::Vector3> verts;
        std::vector<i3d::Vector3> normals;
        
        for ( uint32_t v = 0; v < m_verts.size(); ++v ) {
            i3d::Vector4 wv = transform * i3d::Vector4( m_verts[v], 1 );
            i3d::Vector4 wn = transform * i3d::Vector4( m_normals[v], 0 );
            
            verts.push_back( i3d::Vector3( wv.X(), wv.Y(), wv.Z() ) );
            normals.push_back( i3d::Vector3( wn.X(), wn.Y(), wn.Z() ) );
        }
        
        m_verts = verts;
        m_normals = normals;
        
        CalcBounds();
    }

    //==========================================================================================================================================
    void ToolMesh::FlipTriangles() {

        std::vector<uint32_t> indices;
        indices.reserve( m_triangleIndices.size() );

        for ( uint32_t i = 0; i < m_triangleIndices.size(); i += 3 ) {
            indices.push_back( m_triangleIndices[ i + 2 ] );
            indices.push_back( m_triangleIndices[ i + 1 ] );
            indices.push_back( m_triangleIndices[ i ] );
        }

        m_triangleIndices = indices;
    }

    //==========================================================================================================================================
    void ToolMesh::CalcSmoothNormals() {
        
        // Merge the vertex positions and re-map the triangles so that we
        // have a list of triangles that use the re-mapped positions.
        ToolUniqueList<i3d::Vector3> mergedVerts;
        std::vector<uint32_t>        mergedTris;
        
        for ( uint32_t i = 0; i < m_triangleIndices.size(); i+=3 ) {
           
            uint32_t v0 = mergedVerts.AddItem( m_verts[ m_triangleIndices[i] ] );
            uint32_t v1 = mergedVerts.AddItem( m_verts[ m_triangleIndices[i+1] ] );
            uint32_t v2 = mergedVerts.AddItem( m_verts[ m_triangleIndices[i+2] ] );
            
            mergedTris.push_back( v0 );
            mergedTris.push_back( v1 );
            mergedTris.push_back( v2 );
        }
        
        std::vector<i3d::Vector3> faceNormals;
        CalcFaceNormals( faceNormals );

        // Calculate the vertex normals
        std::vector<i3d::Vector3> vertexNormals;
        for ( uint32_t v = 0; v < mergedVerts.m_items.size(); ++v ) {
            i3d::Vector3 vn = CalcVertexNormal( v, mergedTris, faceNormals );
            vertexNormals.push_back( vn );
        }
        
        // Now that we have the vertex normals, we need to add them into the un-merged arrays
        // in the mesh
        std::vector<bool> haveNormal;
        haveNormal.reserve( m_normals.size() );
        
        for ( uint32_t i = 0; i < m_normals.size(); ++i ) {
            haveNormal.push_back(false);
        }
        
        uint32_t numNormals = 0;
        
        for ( uint32_t i = 0; i < m_triangleIndices.size(); ++ i) {
            uint32_t destIndex = m_triangleIndices[i];
            if ( haveNormal[destIndex] == false ) {
                
                const i3d::Vector3 & srcNormal = vertexNormals[ mergedTris[ i ] ];
                i3d::Vector3 & dstNormal = m_normals[ destIndex ];
                
                dstNormal = srcNormal;
                
                haveNormal[ destIndex ] = true;
                ++numNormals;
            }
        }
    }

    //==========================================================================================================================================
    void ToolMesh::CalcFaceNormals( const std::vector<uint32_t> & tris, const std::vector<i3d::Vector3> & verts, std::vector<i3d::Vector3> & normals ) {
        
        for ( uint32_t i = 0; i < m_triangleIndices.size(); i+=3 ) {
           
            uint32_t v0 = m_triangleIndices[i];
            uint32_t v1 = m_triangleIndices[i+1];
            uint32_t v2 = m_triangleIndices[i+2];
            
            const i3d::Vector3 & vert0 = m_verts[v0];
            const i3d::Vector3 & vert1 = m_verts[v1];
            const i3d::Vector3 & vert2 = m_verts[v2];
            
            i3d::Vector3 d0 = vert0 - vert1;
            i3d::Vector3 d1 = vert0 - vert2;
            
            i3d::Vector3 n;
            n.Cross( d0, d1 );
            n.Normalise();
            
            normals.push_back( n );
        }
    }

    //==========================================================================================================================================
    void ToolMesh::CalcFaceNormals( std::vector<i3d::Vector3> & normals ) {
        CalcFaceNormals( m_triangleIndices, m_verts, normals );
    }

    //==========================================================================================================================================
    i3d::Vector3 ToolMesh::CalcVertexNormal( uint32_t vertex, const std::vector<uint32_t> & tris, const std::vector<i3d::Vector3> & faceNormals ) {
        ToolUniqueList<i3d::Vector3> uniqueNormals;

        for (uint32_t i = 0; i < tris.size(); i+=3) {
            uint32_t currTri = i / 3;
        
            if ( tris[i] == vertex || tris[i+1] == vertex || tris[i+2] == vertex ) {
                uniqueNormals.AddItem( faceNormals[ currTri ] );
            }
        }
        
        assert( uniqueNormals.m_items.empty() == false);
        
        i3d::Vector3 n = i3d::Vector3::ZERO;
        for ( auto & fn : uniqueNormals.m_items ) {
            n += fn;
        }
        
        n.Normalise();
        return n;
    }
}

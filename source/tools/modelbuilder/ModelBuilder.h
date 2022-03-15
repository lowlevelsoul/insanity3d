//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __MODELBUILDER_H__
#define __MODELBUILDER_H__

#include "toolmodel/ToolModel.h"
#include "toolapp/ToolMemStream.h"
#include "i3d/render/ModelStream.h"
#include "SkeletonBuilder.h"

class SkeletonBuilder;

class ModelBuilder {
public:
    class Options {
    public:
        Options() {
            m_scale = 1;
            m_flipFaces = false;
            m_forceStatic = true;
            m_genNormals = false;
            m_stripMixamo = false;
        }

        ~Options() {

        }

    public:
        float                       m_scale;
        bool                        m_flipFaces;
        bool                        m_forceStatic;
        bool                        m_genNormals;
        bool                        m_stripMixamo;
        SkeletonBuilder::Options    m_skeletonOpt;
        
        i3d::stl::Vector<i3d::stl::String::type>::type m_meshFilter;
    };
    
    ModelBuilder( Options & options );
    
    ~ModelBuilder();

    void WriteToStream( ToolMemStream & str, ToolModel * model );
    
    void Build( ToolModel * model );
    
    void GatherMeshes( ToolModel * model );
    
    void GatherMeshes( ToolJoint * joint );
    
    void ModifyMeshes( ToolModel * model );
    
    void WriteMeshes( bool skinned, uint32_t & currVertexCount, uint32_t & currIndexCount );
    
    void WriteMesh( ToolMesh * mesh, bool skinned,
                    uint32_t & currVertexCount, uint32_t & currIndexCount );
    
    void WriteStaticVerts( ToolMesh * mesh );
    
    void WriteIndices( ToolMesh * mesh, uint32_t & currVertexCount, uint32_t & currIndexCount );
    
    void WriteMeshNames();
    
    void WriteMeshSkinWeights( ToolMesh * mesh );

    void WriteHeader( ToolMemStream & str, i3d::ModelStream & header );

    void MakeMeshSkinShell( ToolMesh * mesh, SkeletonBuilder & sb );
    
    bool MeshPassesFilter( const char * name );
    
public:
    class SrcMesh : public RefObject {
    public:
        typedef RefPointer<SrcMesh> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
        
        SrcMesh() {}
        virtual ~SrcMesh() {}
        
        ToolJoint::ref_ptr_t    m_joint;
        ToolMesh::ref_ptr_t     m_mesh;
    };
    
    Options             m_options;
    SrcMesh::vector_t   m_srcMeshes;          ///< The array of meshes that we want to write to build
    ToolMemStream       m_vertexStream;
    ToolMemStream       m_influenceStream;
    ToolMemStream       m_indexStream;
    ToolMemStream       m_meshStream;
    ToolMemStream       m_meshNameHashes;
    ToolMemStream       m_meshNameMap;
    uint32_t            m_vertexCount;
    uint32_t            m_indexCount;
    i3d::Vector3        m_boundsMin;
    i3d::Vector3        m_boundsMax;
    
};

#endif 

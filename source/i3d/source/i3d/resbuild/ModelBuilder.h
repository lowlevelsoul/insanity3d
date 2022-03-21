//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODELBUILDER_H__
#define __I3D_MODELBUILDER_H__

#include "i3d/util/toolmodel/ToolModel.h"
#include "i3d/util/ToolMemStream.h"
#include "i3d/render/ModelStream.h"
#include "i3d/resbuild/SkeletonBuilder.h"

namespace i3d {

    class ModelBuilder : public i3d::ResourceBuilder{
    public:
        RTTI_CLASS_DECLARE( ModelBuilder, ResourceBuilder )
        
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
            //SkeletonCompiler::Options    m_skeletonOpt;
        };
        
        ModelBuilder();
        
        ~ModelBuilder();
        
        virtual void GetDependencies( Resource * resource,
                                      stl::Vector<stl::String::type>::type & inputs ) override;
        
        virtual void Compile( Resource * resource ) override;

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

        void WriteHeader( ToolMemStream & str, ModelStream & header );

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
        
        stl::String::type   m_input;
        stl::Vector<stl::String::type>::type m_meshFilter;
        float               m_scale;
        bool                m_flipFaces;
        bool                m_forceStatic;
        bool                m_genNormals;
        bool                m_stripMixamo;
        
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
        Vector3             m_boundsMin;
        Vector3             m_boundsMax;
    };
}

#endif 

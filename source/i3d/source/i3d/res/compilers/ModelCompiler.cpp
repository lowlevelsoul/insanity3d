//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/res/compilers/ModelCompiler.h"
#include "xe/res/compilers/SkeletonCompiler.h"
#include "xe/util/toolmodel/ModelImporterAssImp.h"

namespace xe {
    
    RTTI_CLASS_BEGIN( ModelCompiler )
        RTTI_PROP( STRING,  "input",            m_input )
        RTTI_PROP( FLOAT,   "scale",            m_scale )
        RTTI_PROP( BOOL,    "flip_faces",       m_flipFaces )
        RTTI_PROP( BOOL,    "force_static",     m_forceStatic )
        RTTI_PROP( BOOL,    "gen_normals",      m_genNormals )
        RTTI_PROP( BOOL,    "strip_mixamo",     m_stripMixamo )
    RTTI_CLASS_END( ModelCompiler )

    class VertexInfluence {
    public:
        VertexInfluence() {
        }
        
        ~VertexInfluence() {
            
        }
        
        void AddInfluence( int32_t joint, float weight) {
            m_joints.push_back( joint );
            m_weights.push_back( weight );
        }
        
        bool IsNormalised() const {
            float total = 0;
            for (auto w : m_weights) {
                total += w;
            }
            
            return std::fabsf(1.0f - total) < 0.0004f;
        }
        
        bool IsNotNormalised() const {
            float total = 0;
            for (auto w : m_weights) {
                total += w;
            }
            
            return (total > 1 || total < 1);
        }
        
        void Normalise() {
            float total = 0;
            for (auto w : m_weights) {
                total += w;
            }
            
            float fac = 1.0f / total;
            
            for (auto & w : m_weights) {
                w *= fac;
            }
        }
        
        void Sort() {
            std::vector<int32_t> sortedJoints;
            std::vector<float> sortedWeights;
            
            for ( uint32_t w = 0; w < m_weights.size(); ++w ) {
                
                if ( sortedWeights.empty() == true ) {
                    sortedJoints.push_back( m_joints[w] );
                    sortedWeights.push_back( m_weights[w] );
                }
                else {
                
                    int32_t insertPos = -1;
                    for (insertPos = 0; insertPos < sortedWeights.size(); ++insertPos) {
                        if ( sortedWeights[ insertPos ] < m_weights[ w ] ) {
                            break;
                        }
                    }

                    sortedJoints.insert( sortedJoints.begin() + insertPos, m_joints[ w ] );
                    sortedWeights.insert( sortedWeights.begin() + insertPos, m_weights[ w ] );
                }
            }
            
            m_joints = sortedJoints;
            m_weights = sortedWeights;
        }
        
        void SortAndPrune( size_t maxInfluences =  4 ) {
            
            Sort();
            if (m_weights.size() > maxInfluences) {
                m_weights.resize( maxInfluences );
                m_joints.resize( maxInfluences );
            }
            Normalise();
        }

        std::vector<int32_t>        m_joints;
        std::vector<float>          m_weights;
    };

    //======================================================================================================================
    ModelCompiler::ModelCompiler() {
        m_vertexCount   = 0;
        m_indexCount    = 0;
        
        m_scale         = 1;
        m_flipFaces     = false;
        m_forceStatic   = true;
        m_genNormals    = false;
        m_stripMixamo   = false;
    }

    //======================================================================================================================
    ModelCompiler::~ModelCompiler() {
        
    }
    
    //======================================================================================================================
    void ModelCompiler::GetDependencies( Resource * resource,
                                        xe::Array<xe::String> & inputs ) {
        inputs.PushBack( m_input );
    }
    
    //======================================================================================================================
    void ModelCompiler::Compile( Resource * resource ) {
        
        // Load the model
        XE_LOG(" Loading source model %s\n", m_input.GetCStr() );
        ModelImporter::Options importOpt;
        ModelImporterAssImp importer( importOpt );
        ToolModel::ref_ptr_t model = importer.Load( m_input.GetCStr() );
        
        XE_LOG("Building model data\n");
        Build( model );
        
        XE_LOG("Compiling model to binary\n");
        ToolMemStream str;
        WriteToStream( str, model );
        XE_LOG("   Size : %uKB", str.Length() / 1024 );
        
        bool folderOk = CreateFolderAtPath( resource->GetPath() );
        XE_ERROR( folderOk == false, " Could not create folder at %s\n", resource->GetPath() );
        
        ScopedFile file( resource->GetPath(), "wb" );
        XE_ERROR( file == nullptr, "Unable to open %s for writing\n", resource->GetPath() );
        
        file->Write( str.GetStream(), str.Length() );
    }

    //======================================================================================================================
    void ModelCompiler::WriteToStream( ToolMemStream & str, ToolModel * model ) {
        
        XE_LOG(" == Model Stats ==\n");
        XE_LOG("   Mesh Count : %u\n", model->m_meshes.size());
        XE_LOG("      Skinned : %s\n", model->HasSkinnedMeshes() == true ? "YES" : "NO" );
        XE_LOG("   Min Bounds : <%4.4f, %4.4f, %4.4f>\n", m_boundsMin.X(), m_boundsMin.Y(), m_boundsMin.Z() );
        XE_LOG("   Max Bounds : <%4.4f, %4.4f, %4.4f>\n", m_boundsMax.X(), m_boundsMax.Y(), m_boundsMax.Z() );
        XE_LOG("   Dimensions : <%4.4f, %4.4f, %4.4f>\n", m_boundsMax.X() -  m_boundsMin.X(),
                                                          m_boundsMax.Y() -  m_boundsMin.Y(),
                                                          m_boundsMax.Z() -  m_boundsMin.Z() );

        xe::ModelStream header;
        memset( &header, 0, sizeof( header ) );
        header.m_version        = xe::ModelStream::VERSION;
        header.m_indexCount     = m_indexCount;
        header.m_vertexCount    = m_vertexCount;
        header.m_bounds[ 0 ][ 0 ] = m_boundsMin.X();
        header.m_bounds[ 0 ][ 1 ] = m_boundsMin.Y();
        header.m_bounds[ 0 ][ 2 ] = m_boundsMin.Z();
        header.m_bounds[ 1 ][ 0 ] = m_boundsMax.X();
        header.m_bounds[ 1 ][ 1 ] = m_boundsMax.Y();
        header.m_bounds[ 1 ][ 2 ] = m_boundsMax.Z();
        header.m_meshCount      = ( uint32_t ) model->m_meshes.size();
        header.m_indexDataSize  = ( uint32_t ) m_indexStream.Length();
        header.m_vertexDataSize = ( uint32_t ) m_vertexStream.Length();
        
        if ( model->HasSkinnedMeshes() == true ) {
            header.m_flags |= xe::ModelStream::F_SKINNED;
        }

        uintptr_t headerPos = str.Tell();
        WriteHeader( str, header );

        str.PadToAlignment( 16 );
        header.m_offsMeshes = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_meshStream.GetStream(), m_meshStream.Length() );

        str.PadToAlignment( 16 );
        header.m_offsMeshHashArray = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_meshNameHashes.GetStream(), m_meshNameHashes.Length() );

        str.PadToAlignment( 16 );
        header.m_offsMeshHashMap = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_meshNameMap.GetStream(), m_meshNameMap.Length() );

        str.PadToAlignment( 16 );
        header.m_offsVertices = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_vertexStream.GetStream(), m_vertexStream.Length() );
        
        str.PadToAlignment( 16 );
        header.m_offsInfluences = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_influenceStream );

        str.PadToAlignment( 16 );
        header.m_offsIndices = ( uint32_t ) ( str.Tell() - headerPos );
        str.Write( m_indexStream.GetStream(), m_indexStream.Length() );

        uintptr_t end = str.Tell();
        str.Seek( headerPos );
        WriteHeader( str, header );
        str.Seek( end );
    }

    //======================================================================================================================
    void ModelCompiler::WriteHeader( ToolMemStream & str, xe::ModelStream & header ) {

        str.Write( &header.m_version, 1 );
        str.Write( &header.m_flags, 1 );
        str.Write( &header.m_vertexCount, 1 );
        str.Write( &header.m_indexCount, 1 );
        str.Write( &header.m_meshCount, 1 );
        str.Write( &header.m_vertexDataSize, 1 );
        str.Write( &header.m_indexDataSize, 1 );
        str.Write( &header.m_offsMeshes, 1 );
        str.Write( &header.m_offsMeshHashArray, 1 );
        str.Write( &header.m_offsMeshHashMap, 1 );
        str.Write( &header.m_offsVertices, 1 );
        str.Write( &header.m_offsIndices, 1 );
        str.Write( &header.m_offsInfluences, 1 );
        str.Write( &header.m_pad, 1 );
        str.Write( header.m_bounds[ 0 ], 3 );
        str.Write( header.m_bounds[ 1 ], 3 );

    }

    //======================================================================================================================
    void ModelCompiler::Build( ToolModel * model ) {
        // Build a list of the meshes that we want to compile
        GatherMeshes( model );
        XE_ERROR(m_srcMeshes.empty() == true, "No meshes in scene!");

        if ( model->HasSkinnedMeshes() == true ) {
            XE_LOG(" Building skinned model \n");
        }
        
        // Modify the gathered meshes
        ModifyMeshes( model );
        
        // Write out the Meshes into seperate binary streams
        WriteMeshes( model->HasSkinnedMeshes(), m_vertexCount, m_indexCount );
    }

    //======================================================================================================================
    void ModelCompiler::GatherMeshes( ToolModel * model ) {
        
        // Gather (and make copies of) any meshes in the scene
        GatherMeshes( model->m_skeleton->m_root );
    }

    //======================================================================================================================
    void ModelCompiler::ModifyMeshes( ToolModel * model ) {

        SkeletonCompiler sb;
        sb.m_scale = m_scale;
        sb.m_pruneMixamoMarkers = m_stripMixamo;

        if ( model->HasSkinnedMeshes() ) {
            sb.BuildJointList( model->m_skeleton );
        }

        //
        for ( auto sm : m_srcMeshes ) {
            
            XE_LOG("Processing '%s'\n", sm->m_mesh->m_name.c_str());

            // Flip the winding order of the triangles if needed
            if ( m_flipFaces == true ) {
                XE_LOG("    Flipping triangles\n");
                sm->m_mesh->FlipTriangles();
            }
            
            // Calculate smooth vertex normals
            if ( m_genNormals == true ) {
                XE_LOG("    Calculating vertex normals\n");
                sm->m_mesh->CalcSmoothNormals();
            }
            
            // Apply the effects of the skin offset matrices to the mesh
            if ( sm->m_mesh->IsSkinned()) {
                XE_LOG("    Building skinned mesh shell\n");
                MakeMeshSkinShell(sm->m_mesh, sb);
            }
            else {
                XE_LOG("    Transforming static into global space\n");
                // Transform the mesh into global-space
                sm->m_mesh->TransformByJoint( sm->m_joint.Get() );
            }
                    
            // Apply any transforms from the caller
            xe::Matrix4 modTransform = xe::Matrix4::IDENTITY;
            modTransform.Set(m_scale, m_scale, m_scale, 1);
         
            sm->m_mesh->ApplyTransform( modTransform );
        }

        // Calculate entire model bounds
        m_boundsMin = m_srcMeshes[ 0 ]->m_mesh->m_boundsMin;
        m_boundsMax = m_srcMeshes[ 0 ]->m_mesh->m_boundsMax;

        for ( auto sm : m_srcMeshes ) {
            m_boundsMin.Min( m_boundsMin, sm->m_mesh->m_boundsMin );
            m_boundsMax.Max( m_boundsMax, sm->m_mesh->m_boundsMax );
        }
    }

    //======================================================================================================================
    void ModelCompiler::MakeMeshSkinShell( ToolMesh * mesh, SkeletonCompiler & sb ) {
        
        // Sort the skin info by vertex
        std::vector<VertexInfluence> influences;
        influences.resize( mesh->m_verts.size() );
        
        std::vector<xe::Matrix4> boneOffsets;
        boneOffsets.resize( sb.m_joints.GetSize() );
        
        for ( auto c : mesh->m_skin->m_clusters ) {
            int32_t clusterJointIndex = sb.FindJoint( c->m_joint.c_str() );
            
            boneOffsets[ clusterJointIndex ] = c->m_offset;
            
            for ( uint32_t v = 0; v < c->m_verts.size(); ++v ) {
                uint32_t vertIndex = c->m_verts[ v ];
                float weight = c->m_weights[ v ];
                
                influences[ vertIndex ].AddInfluence( clusterJointIndex , weight);
            }
        }
        
        std::vector<xe::Vector3> shellVerts;
        std::vector<xe::Vector3> shellNorms;
        
        for ( auto & i : influences ) {
            if ( i.IsNotNormalised() ) {
                i.Normalise();
            }
        }
        
        int32_t vertex = 0;
        for ( auto & i : influences ) {
            xe::Vector3 vaccum = xe::Vector3::ZERO;
            xe::Vector3 naccum = xe::Vector3::ZERO;
            
            for ( uint32_t e = 0; e < i.m_weights.size(); ++e) {
                int32_t jointIndex = i.m_joints[e];
                xe::Matrix4 & jointOffset = boneOffsets[ jointIndex ];
                xe::Matrix4 & globalOffset = sb.m_globalMats[ jointIndex ];
            
                // Transform normal and position into joint local space
                xe::Vector4 sv = jointOffset * xe::Vector4( mesh->m_verts[ vertex ], 1 );
                xe::Vector4 sn = jointOffset * xe::Vector4( mesh->m_normals[ vertex ], 0 );

                // Transform normal and position into global space
                xe::Vector4 svg = globalOffset * sv;
                xe::Vector4 sng = globalOffset * sn;
                
                svg *= i.m_weights[e];
                
                vaccum += svg;
                naccum += sng;
            }
            
            naccum.Normalise();
            
            shellVerts.push_back( vaccum );
            shellNorms.push_back( naccum );
            
            ++vertex;
        }
        
        mesh->m_verts = shellVerts;
        mesh->m_normals = shellNorms;

        mesh->CalcBounds();
    }

    //======================================================================================================================
    void ModelCompiler::GatherMeshes( ToolJoint * joint ) {
        
        if (joint->HasMeshes() == true ) {
            if (joint->m_meshes.size() > 1) {
                
                for (uint32_t m = 0; m < joint->m_meshes.size(); ++m) {
                    SrcMesh * srcMesh = new SrcMesh;
                    
                    srcMesh->m_joint = joint;
                    
                    srcMesh->m_mesh = new ToolMesh( nullptr );
                    srcMesh->m_mesh->Copy( joint->m_meshes[m] );
                    srcMesh->m_mesh->m_model = joint->m_meshes[m]->m_model;
                    
                    xe::String meshName = sys->Vformat("%s_%04u", joint->m_name.c_str(), m);
                    srcMesh->m_mesh->m_name = meshName.GetCStr();
                    
                    m_srcMeshes.push_back( srcMesh );
                }
            }
            else {
                SrcMesh * srcMesh = new SrcMesh;
                
                srcMesh->m_joint = joint;
                
                srcMesh->m_mesh = new ToolMesh( nullptr );
                srcMesh->m_mesh->Copy( joint->m_meshes[0] );
                srcMesh->m_mesh->m_name = joint->m_name;
                srcMesh->m_mesh->m_model = joint->m_meshes[0]->m_model;
                
                m_srcMeshes.push_back( srcMesh );
            }
        }
        
        for ( auto c : joint->m_children ) {
            GatherMeshes( c );
        }
    }

    //======================================================================================================================
    void ModelCompiler::WriteMeshes( bool skinned, uint32_t & currVertexCount, uint32_t & currIndexCount ) {
        
        currVertexCount = 0;
        currIndexCount = 0;
        
        // Write our the meshes
        for ( auto sm : m_srcMeshes ) {
            WriteMesh( sm->m_mesh, skinned, currVertexCount, currIndexCount );
        }
        
        // Write the mesh name map
        WriteMeshNames();
    }

    //======================================================================================================================
    void ModelCompiler::WriteMesh( ToolMesh * mesh, bool skinned,
                                 uint32_t & currVertexCount, uint32_t & currIndexCount ) {
        
        XE_ERROR( skinned == true && mesh->IsSkinned() == false, "Skinned model contains a mesh (%s) that is not skinned\n", mesh->m_name.c_str() );
        
        // Write vertices
        WriteStaticVerts( mesh );
        
        if ( skinned == true ) {
            WriteMeshSkinWeights( mesh );
        }
        
        // Write indices
        WriteIndices( mesh, currVertexCount, currIndexCount );

        
        // Write mesh stream
        xe::MeshStream meshInfo;
        meshInfo.m_indexStart = currIndexCount;
        meshInfo.m_indexCount = (uint32_t) mesh->m_triangleIndices.size();
        meshInfo.m_vertexStart = currVertexCount;
        meshInfo.m_vertexCount = (uint32_t) mesh->m_verts.size();
        
        m_meshStream.Write( &meshInfo.m_vertexStart, 1 );
        m_meshStream.Write( &meshInfo.m_vertexCount, 1 );
        m_meshStream.Write( &meshInfo.m_indexStart, 1 );
        m_meshStream.Write( &meshInfo.m_indexCount, 1 );
        
        currVertexCount += (uint32_t) mesh->m_verts.size();
        currIndexCount += (uint32_t) mesh->m_triangleIndices.size();
    }

    //======================================================================================================================
    void ModelCompiler::WriteStaticVerts( ToolMesh * mesh ) {
        
        // Write out static vertex data
        for ( uint32_t v = 0; v < mesh->m_verts.size(); ++v) {
            
            float posTexU[4] = {
                mesh->m_verts[ v ].X(),
                mesh->m_verts[ v ].Y(),
                mesh->m_verts[ v ].Z(),
                mesh->m_texcoords[ v ].X()
            };

            float normTexU[4] = {
                mesh->m_normals[ v ].X(),
                mesh->m_normals[ v ].Y(),
                mesh->m_normals[ v ].Z(),
                mesh->m_texcoords[ v ].Y()
            };
            
            m_vertexStream.Write( posTexU, 4 );
            m_vertexStream.Write( normTexU, 4 );
        }
    }

    //======================================================================================================================
    void ModelCompiler::WriteIndices( ToolMesh * mesh, uint32_t & currVertexCount, uint32_t & currIndexCount ) {
        
        for ( uint32_t i = 0; i < mesh->m_triangleIndices.size(); i+=3 ) {
            
            uint32_t tris[3];
           
            tris[ 0 ] = mesh->m_triangleIndices[ i ];
            tris[ 1 ] = mesh->m_triangleIndices[ i + 1 ];
            tris[ 2 ] = mesh->m_triangleIndices[ i + 2 ];
            
           /*tris[ 0 ] += currVertexCount;
            tris[ 1 ] += currVertexCount;
            tris[ 2 ] += currVertexCount; */
            
            m_indexStream.Write( tris, 3 );
        }
    }

    //======================================================================================================================
    void ModelCompiler::WriteMeshNames( ) {
        
        std::vector<uint64_t> hashes;
        std::vector<int32_t> map;
        
        uint32_t meshIndex = 0;

        for ( auto sm : m_srcMeshes ) {
            uint64_t hash = xe::fh64::CalcFromString( sm->m_mesh->m_name.c_str() );
            
            if ( hashes.empty() == true ) {
                hashes.push_back( hash );
                map.push_back( meshIndex );
            }
            else {
                int32_t index;
                xe::Sys::BinarySearch<uint64_t, int32_t>( index, hash, &hashes[0], hashes.size() );
                
                hashes.insert( hashes.begin() + index, hash );
                map.insert( map.begin() + index, meshIndex );
            }
            
            ++meshIndex;
        }
        
        m_meshNameHashes.Write( &hashes[0], hashes.size() );
        m_meshNameMap.Write( &map[0], map.size() );
    }

    //======================================================================================================================
    void ModelCompiler::WriteMeshSkinWeights( ToolMesh * mesh ) {
        
        SkeletonCompiler sb;
        sb.m_scale = m_scale;
        sb.m_pruneMixamoMarkers = m_stripMixamo;
        
        sb.BuildJointList( mesh->m_model->m_skeleton );
        
        XE_LOG("Building weights for mesh %s. Skeleton has %u joints\n", mesh->m_name.c_str(), sb.m_joints.GetSize() );
        
        // Sort the skin info by vertex
        std::vector<VertexInfluence> influences;
        influences.resize( mesh->m_verts.size() );
        
        std::vector<xe::Matrix4> boneOffsets;
        boneOffsets.resize( sb.m_joints.GetSize() );
        
        for ( auto c : mesh->m_skin->m_clusters ) {
            int32_t clusterJointIndex = sb.FindJoint( c->m_joint.c_str() );
            XE_ERROR( clusterJointIndex < 0, "Could not find joint for cluster %s\n", c->m_joint.c_str() );
            
            boneOffsets[ clusterJointIndex ] = c->m_offset;
            
            for ( uint32_t v = 0; v < c->m_verts.size(); ++v ) {
                uint32_t vertIndex = c->m_verts[ v ];
                float weight = c->m_weights[ v ];
                
                influences[ vertIndex ].AddInfluence( clusterJointIndex , weight);
            }
        }
        
        // Clamp the weights to have a sum of 1
        for (auto & i : influences ) {
            i.SortAndPrune(4);
        }
        
        // Fix anu vertices that have no influences. We do this by copying the influence
        // info from a neiboughring verte.
        uint32_t index = 0;
        for ( auto & i : influences ) {
            if ( i.m_weights.empty() == true ) {
                XE_LOG( "Found a skinned vertex with no influences." );
                
                if (index > 0) {
                    XE_LOG(" Using previous vertex.\n");
                    i = influences[ index - 1];
                }
                else if (index < influences.size()-1 ) {
                    XE_LOG(" Using next vertex.\n");
                    i = influences[ index + 1 ];
                }
                else {
                    XE_ERROR( false, " Unable to fix skinned vertex with no influecnes\n" ) ;
                }
            }
            
            ++index;
        }

        // Now Write out weight info
        for (auto & i : influences ) {
            uint32_t joints[4] = {0, 0, 0, 0};
            float weights[4] = {0, 0, 0, 0};
            uint32_t params[4] = {0, 0, 0, 0};
            
            for ( uint32_t w = 0; w < i.m_joints.size(); ++w) {
                joints[w] = i.m_joints[w];
                weights[w] = i.m_weights[w];
            }
            
            params[0] = (uint32_t) i.m_joints.size();
            
            m_influenceStream.Write( joints, 4 );
            m_influenceStream.Write( weights, 4 );
            m_influenceStream.Write( params, 4 );
        }
    }
}

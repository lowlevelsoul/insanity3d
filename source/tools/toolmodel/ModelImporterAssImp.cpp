//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "toolmodel/ModelImporterAssImp.h"
//#include "assimp/pbrmaterial.h"

//==========================================================================================================================================
ModelImporterAssImp::ModelImporterAssImp( Options & options ) : ModelImporter( options ) {
    m_scene = nullptr;
    m_importer = nullptr;

    m_model = new ToolModel;
}

//==========================================================================================================================================
ModelImporterAssImp::~ModelImporterAssImp() {
    SAFE_DELETE( m_importer );
}

//==========================================================================================================================================
ToolModel::ref_ptr_t ModelImporterAssImp::Load( const char * path ) {
    
    LoadScene( path );

    ImportMaterials();

    ImportMeshes();

    ImportSkeleton();
    
    ImportAnim( (aiScene*) m_scene );
         
    return m_model;
}

//==========================================================================================================================================
bool ModelImporterAssImp::LoadScene( const char * path ) {
    i3d::stl::String::type fullPath;
    fs->MakeFullPath( fullPath, path);
    
    uint32_t importFlags = /*aiProcess_GenSmoothNormals |*/ aiProcess_RemoveRedundantMaterials /*| aiProcess_CalcTangentSpace*/ | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_MakeLeftHanded;
    
    m_importer = new Assimp::Importer;
    
    m_scene = m_importer->ReadFile( fullPath.c_str(), importFlags);
    XE_ERROR( m_scene == nullptr, "Error loading scene.\n%s\n", m_importer->GetErrorString() );
    
    return true;
}

//==========================================================================================================================================
void ModelImporterAssImp::ImportMaterials() {
    for ( uint32_t m = 0; m != m_scene->mNumMaterials; ++m ) {
        aiMaterial * srcMat = m_scene->mMaterials[ m ];

        ToolMaterial * mat = m_model->AddMaterial( srcMat->GetName().C_Str() );

        aiString texPath;
        aiReturn res;
        
        res = srcMat->GetTexture( aiTextureType_DIFFUSE, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_ALBEDO ] = texPath.C_Str();
        }

        res = srcMat->GetTexture( aiTextureType_METALNESS, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_METALLIC ] = texPath.C_Str();
        }

        res = srcMat->GetTexture( aiTextureType_AMBIENT_OCCLUSION, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_AMBIENT_OCCLUSION ] = texPath.C_Str();
        }

        res = srcMat->GetTexture( aiTextureType_AMBIENT, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_AMBIENT ] = texPath.C_Str();
        }

        res = srcMat->GetTexture( aiTextureType_EMISSIVE, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_GLOW ] = texPath.C_Str();
        }

        res = srcMat->GetTexture( aiTextureType_SPECULAR, 0, &texPath );
        if ( res == aiReturn_SUCCESS ) {
            mat->m_textures[ ToolMaterial::STAGE_SPECULAR ] = texPath.C_Str();
        }
    }
}


//==========================================================================================================================================
void ModelImporterAssImp::ImportSkeleton() {
    m_model->AddSkeleton();

    ImportJoint( m_scene->mRootNode );
}

//==========================================================================================================================================
void ModelImporterAssImp::ImportJoint( aiNode * node ) {

    // Get the parent joint from the skeleton
    ToolJoint * parent = nullptr;
    if ( node->mParent != nullptr ) {
        parent = m_model->m_skeleton->FindJoint( node->mParent->mName.C_Str() );
        XE_ERROR( parent == nullptr, "Error finding parent joint '%s' from joint '%s'\n", node->mName.C_Str(), node->mParent->mName.C_Str() );
    }

    // Add the joint to the skeleton and ste the transform
    ToolJoint * joint = m_model->m_skeleton->AddJoint( node->mName.C_Str(), parent );
    joint->m_transform = Convert( node->mTransformation );

    // Attach any meshes
    for ( uint32_t m = 0; m < node->mNumMeshes; ++m ) {
        joint->m_meshes.push_back( m_model->m_meshes[ node->mMeshes[ m ] ] );
    }

    // Recurse down though child nodes
    for ( uint32_t c = 0; c < node->mNumChildren; ++c ) {
        ImportJoint( node->mChildren[ c ] );
    }
}

//==========================================================================================================================================
void ModelImporterAssImp::ImportMeshes() {
    for ( uint32_t m = 0; m < m_scene->mNumMeshes; ++m ) {
        ImportMesh( m_scene->mMeshes[ m ] );
    }
}

//==========================================================================================================================================
void ModelImporterAssImp::ImportMesh( aiMesh * srcMesh ) {

    ToolMesh * mesh = m_model->AddMesh( srcMesh->mName.C_Str() );

    // Set the mesh material
    mesh->SetMaterial( m_scene->mMaterials[ srcMesh->mMaterialIndex ]->GetName().C_Str() );
    
    // Add triangles to the mesh
    for(uint32_t p=0; p < srcMesh->mNumFaces; ++p) {
        aiFace * face = &srcMesh->mFaces[p];
        XE_ERROR( face->mNumIndices != 3, "Encountered a face in mesh '%s' that is not a triangle\n", srcMesh->mName.C_Str());

        mesh->m_triangleIndices.push_back( face->mIndices[ 0 ] );
        mesh->m_triangleIndices.push_back( face->mIndices[ 1 ] );
        mesh->m_triangleIndices.push_back( face->mIndices[ 2 ] );
    }

    // Import verts and normals
    
    for(uint32_t v=0; v < srcMesh->mNumVertices; ++v) {
        aiVector3D srcP = srcMesh->mVertices[v];
        
        i3d::Vector3 p(srcP.x, srcP.y, srcP.z);
        mesh->m_verts.push_back(p);
    }
    
    for(uint32_t v=0; v < srcMesh->mNumVertices; ++v) {
        aiVector3D srcN = srcMesh->mNormals[v];
        i3d::Vector3 n(srcN.x, srcN.y, srcN.z);
        mesh->m_normals.push_back(n);
    }
    
    // Import texture coordinates.
    
    // Find the first valid set of texture coordinates
    int32_t texCoordSet = -1;
    for(uint32_t i=0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
        if (srcMesh->mTextureCoords[i] != nullptr) {
            texCoordSet = i;
            break;
        }
    }
    
    if (texCoordSet < 0) {
        // No valid texture coordinates, so just set
        // all of the texture coords to be zero
        
        for(uint32_t v=0; v < srcMesh->mNumVertices; ++v) {
            mesh->m_texcoords.push_back( i3d::Vector3::ZERO);
        }
    }
    else {
        // We have a valid texture coordinate set, so copy it across
        // to the mesh
        aiVector3D* texCoords = srcMesh->mTextureCoords[texCoordSet];
        
        for(uint32_t t=0; t < srcMesh->mNumVertices; ++t) {
            i3d::Vector3 coord(texCoords[t].x, 1.0f - texCoords[t].y, texCoords[t].z);
            mesh->m_texcoords.push_back(coord);
        }
    }

    // Add skinning information
    if ( srcMesh->mNumBones > 0 ) {
        ToolSkin * skin = mesh->AddSkin();

        for ( uint32_t b = 0; b < srcMesh->mNumBones; ++b ) {
            aiBone * bone = srcMesh->mBones[ b ];
            ToolSkinCluster * cluster = skin->AddCluster();

            cluster->m_joint = bone->mName.C_Str();
            cluster->m_offset = Convert( bone->mOffsetMatrix );
                        
            for ( uint32_t w = 0; w < bone->mNumWeights; ++w ) {
                
                if ( bone->mWeights[w].mWeight >= 0 ) {
                    cluster->m_verts.push_back( bone->mWeights[w].mVertexId );
                    cluster->m_weights.push_back( bone->mWeights[w].mWeight );
                }
                
            }
        }
    }
    
    mesh->CalcBounds();
}

//==========================================================================================================================================
void ModelImporterAssImp::ImportAnim( aiScene * scene ) {
    if ( scene->mNumAnimations == 0 ) {
        return;
    }
    
    aiAnimation * anim = scene->mAnimations[ 0 ];
    ToolAnimation * dstAnim = m_model->AddAnimation();
    
    dstAnim->m_framesPerSecond = (uint32_t) anim->mTicksPerSecond;
    dstAnim->m_frameCount = (uint32_t) anim->mDuration;
    dstAnim->m_flags.m_absolute = 1;
    
    for( uint32_t n = 0; n < anim->mNumChannels; ++n) {
        
        aiNodeAnim * nodeSrc = anim->mChannels[ n ];
        
        uint32_t numPosKeys = nodeSrc->mNumPositionKeys;
        uint32_t numRotKeys = nodeSrc->mNumRotationKeys;
        
        ToolAnimNode * nodeDst = dstAnim->AddNode( nodeSrc->mNodeName.C_Str() );
        
        for ( uint32_t f =0; f < dstAnim->m_frameCount; ++f) {

            int32_t index = ( f >= numPosKeys ) ? nodeSrc->mNumPositionKeys - 1 : f;
            i3d::Vector3 p( nodeSrc->mPositionKeys[index].mValue.x,
                            nodeSrc->mPositionKeys[index].mValue.y,
                            nodeSrc->mPositionKeys[index].mValue.z );
            
            nodeDst->m_trackTranslate.push_back(p);
        }
        
        for ( uint32_t f =0; f < dstAnim->m_frameCount; ++f) {
            
            int32_t index = ( f >= numRotKeys ) ? nodeSrc->mNumRotationKeys - 1 : f;
            i3d::Quaternion q( nodeSrc->mRotationKeys[index].mValue.x,
                               nodeSrc->mRotationKeys[index].mValue.y,
                               nodeSrc->mRotationKeys[index].mValue.z,
                               nodeSrc->mRotationKeys[index].mValue.w );
            q.Normalise( q );
            
            i3d::Matrix4 xform;
            xform.SetRotationQ( q );
            q.SetFromMatrix( xform );
            
            nodeDst->m_trackRotate.push_back(q);
        }
    }
}

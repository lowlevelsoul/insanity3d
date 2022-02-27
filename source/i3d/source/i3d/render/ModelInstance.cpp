//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/ModelInstance.h"
#include "i3d/render/Skeleton.h"

namespace i3d {
    
    //======================================================================================================================
    ModelInstance::ModelInstance() {
        m_model = nullptr;
        m_meshCount = 0;
        
        m_skeleton =  nullptr;
        m_jointCount = 0;
    }
    
    //======================================================================================================================
    ModelInstance::~ModelInstance() {
    
    }
    
    //======================================================================================================================
    void ModelInstance::Create( Model * model ) {
        m_model = model;
        m_meshes.Resize( model->GetMeshCount() );
        m_meshCount = model->GetMeshCount();
        
        for ( uint32_t i = 0; i < m_meshCount; ++i ) {
            m_meshes[i].m_mesh = &model->GetMeshArray()[i];
            m_meshes[i].m_visible = 1;
            m_meshes[i].m_material = nullptr;
        }
        
        for (uint32_t i = 0; i < m_jointCount; ++i ) {
            m_animMatrices[i] = Matrix4::IDENTITY;
        }
    }
    
    //======================================================================================================================
    void ModelInstance::Create( Model * model, Skeleton * skeleton ) {
        Create( model );
        
        m_skeleton = skeleton;
        m_jointCount = m_skeleton->GetJointCount();
        
        m_bodyMatrices.Resize( m_jointCount );
        m_animMatrices.Resize(  m_jointCount );
        
        for ( uint32_t j = 0; j < m_jointCount; ++j ) {
            m_animMatrices[ j ] = Matrix4::IDENTITY;
        }
    }
    
    //======================================================================================================================
    int32_t ModelInstance::FindMeshIndex( const char * name ) {
        return m_model->FindMeshIndex( name );
    }
    
    //======================================================================================================================
    void ModelInstance::SetMeshVisible( int32_t index, bool visible ) {
        XE_ASSERT(index >= 0 && index < m_meshCount);
        m_meshes[ index ].m_visible = ( visible == true ) ? 1 : 0;
    }
    
    //======================================================================================================================
    void ModelInstance::SetMeshMaterial( int32_t index, Material * material ) {
        XE_ASSERT(index >= 0 && index < m_meshCount);
        m_meshes[ index ].m_material = material;
    }
    
    //======================================================================================================================
    void ModelInstance::SetMaterial( Material * material ) {
        for ( int32_t m = 0; m < m_meshCount; ++m ) {
            m_meshes[ m ].m_material = material;
        }
    }
    
    //======================================================================================================================
    int32_t ModelInstance::FindJointIndex( const char * name ) const {
        XE_ASSERT( m_skeleton != nullptr );
        return m_skeleton->FindJoint( name );
    }

    //======================================================================================================================
    void ModelInstance::SetNamedMeshesVisible( const char * names ) {
        
        // Set all of the meshes to be invisible
        for ( int32_t m = 0; m < m_meshCount; ++m ) {
            m_meshes[ m ].m_visible = 0;
        }
        
        // Iterate through the name list and set the named meshes to
        // be visisble
        static constexpr size_t MAX_NAME_SIZE = 128;
        char nameBuff[ MAX_NAME_SIZE + 1 ];
        const char * nameBegin = names;
        
        bool gotItem = true;
        do {
            gotItem = Sys::GetNextStringItem( nameBegin, nameBuff, MAX_NAME_SIZE);
            if ( gotItem == true ) {
                
                int32_t meshIndex = FindMeshIndex( nameBuff );
                if ( meshIndex >= 0 ) {
                    m_meshes[ meshIndex ].m_visible = 1;
                }
            }
        } while ( gotItem );
    }
    
    //======================================================================================================================
    void ModelInstance::UpdateMatrices( bool useSkeleton ) {
        
        XE_ASSERT( m_skeleton != nullptr );
        
        const int32_t * jointParent = m_skeleton->GetJointParents();
        const Matrix4 * jointLocal = m_skeleton->GetJointLocalMatrices();
        const Matrix4 * anim = &m_animMatrices[0];
        Matrix4 * bodyMat = &m_bodyMatrices[0];
        
        if ( useSkeleton == true) {
        
            for( uint32_t j = 0; j < m_jointCount; ++j ) {
                
                if (*jointParent < 0) {
                    *bodyMat = *anim * *jointLocal;
                }
                else {
                    Matrix4 animLocal = *anim * *jointLocal;
                    *bodyMat = animLocal * m_bodyMatrices[ *jointParent ];
                }
                
                ++jointParent;
                ++jointLocal;
                ++anim;
                ++bodyMat;
            }
        }
        else {
            for( uint32_t j = 0; j < m_jointCount; ++j ) {
                
                if (*jointParent < 0) {
                    *bodyMat = *anim;
                }
                else {
                    *bodyMat = *anim * m_bodyMatrices[ *jointParent ];
                }
                
                ++jointParent;
                ++anim;
                ++bodyMat;
            }
        }
    }
}



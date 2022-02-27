//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODELINSTANCE_H__
#define __I3D_MODELINSTANCE_H__

#include "i3d/core/Types.h"
#include "i3d/render/Model.h"
#include "i3d/render/Material.h"

namespace i3d {
    
    class Skeleton;
    
    class ModelInstance {
    public:
        struct Mesh {
            Material *      m_material;
            uint64_t        m_visible;
        };
        
        ModelInstance();
        
        ~ModelInstance();
        
        void Create( Model * model );
        
        void Create( Model * model, Skeleton * skeleton );
        
        int32_t FindMeshIndex( const char * name );
        
        void SetMeshVisible( int32_t index, bool visible );
        
        void SetMeshMaterial( int32_t index, Material * material );
        
        void SetMaterial( Material * material );
        
        void SetNamedMeshesVisible( const char * names );
        
        void UpdateMatrices( bool useSkeleton = true );
        
        size_t GetMeshCount() const { return m_meshCount; }
        
        Mesh * GetMeshAt( int32_t index ) {
            XE_ASSERT( index < m_meshCount );
            return &m_meshes[ index ];
        }
        
        const Mesh * GetMeshAt( int32_t index ) const {
            XE_ASSERT( index < m_meshCount );
            return &m_meshes[ index ];
        }
        
        Model * GetModel() { return m_model; }
        
        const Model * GetModel() const { return m_model; }
        
        Skeleton * GetSkeleton() { return m_skeleton; }
        
        const Skeleton * GetSkeleton() const { return m_skeleton; }
        
        Matrix4 * GetBodyMatrices() { return m_bodyMatrices.Begin(); }
        
        const Matrix4 * GetBodyMatrices() const { return m_bodyMatrices.Begin(); }
        
        Matrix4 * GetAnimMatrices() { return m_animMatrices.Begin(); }
        
        const Matrix4 * GetAnimMatrices() const { return m_animMatrices.Begin(); }
        
        int32_t FindJointIndex( const char * name ) const;
        
        bool IsSkinned() const { return m_skeleton != nullptr; }

    protected:
        Model *                 m_model;
        xe::Array<Mesh>         m_meshes;
        size_t                  m_meshCount;
        
        Skeleton *              m_skeleton;
        xe::Array<Matrix4>      m_bodyMatrices;
        xe::Array<Matrix4>      m_animMatrices;
        size_t                  m_jointCount;
        
    };
}


#endif

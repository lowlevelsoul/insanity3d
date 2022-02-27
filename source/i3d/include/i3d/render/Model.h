//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODEL_H__
#define __I3D_MODEL_H__

#include "i3d/core/Types.h"
#include "i3d/math/Math3dTypes.h"
#include "i3d/res/Resource.h"

namespace i3d {
    
    class Model : public Resource {
    public:
        static const uint64_t F_SKINNED = 1;
        
        virtual ~Model() {}
        
        virtual void Create( size_t vertexCount, size_t indexCount, size_t meshCount, uint64_t flags = 0) = 0;
        
        /// Write vertex data to a model
        virtual void WriteVertices( const void * src, uintptr_t start, size_t count ) = 0;

        /// Write index data to a model
        virtual void WriteIndices( const void * src, uintptr_t start, size_t count ) = 0;
        
        /// Write vertex skin influence data to the mesh
        virtual void WriteInfluences( const void * src, uint32_t start, size_t count ) = 0;

        /// Set the axis-aloigned bounding box of the model
        void SetBounds( const Vector3& bmin, const Vector3& bmax ) {
            m_boundsMin = bmin;
            m_boundsMax = bmax;
        }

        ///Retreive the axis-aligned bounding box of the model
        void GetBounds( Vector3& bmin, Vector3& bmax ) {
            bmin = m_boundsMin;
            bmax = m_boundsMax;
        }
        
        size_t GetVertexCount() const { return m_vertexCount; }
        size_t GetVertexStride() const { return m_vertexStride; }
        size_t GetVertexSize() const { return m_vertexSize; }
        
        size_t GetIndexCount() const { return m_indexCount; }
        size_t GetIndexStride() const { return m_indexStride; }
        size_t GetIndexSize() const { return m_indexSize; }
        
    protected:
        size_t      m_vertexCount;
        size_t      m_vertexStride;
        size_t      m_vertexSize;
        
        size_t      m_indexCount;
        size_t      m_indexStride;
        size_t      m_indexSize;
        
        Vector3     m_boundsMin;
        Vector3     m_boundsMax;
    };
};

namespace i3d { namespace model {
    
    extern handle_t (* Alloc )();
    
    extern handle_t (* Create )( handle_t self_, size_t vertexCount, size_t indexCount, uint64_t flags );

	/// Create a model with a given number of vertices and indices
    extern handle_t	(* AllocAndCreate )( size_t vertexCount, size_t indexCount, uint64_t flags );
    
    /// Load the model from a file given by a path
    extern handle_t (* Load )( const char * path );
    
	/// Destroy a model 
    extern void (* Destroy )( handle_t self_ );

    /// Write vertex data to a model
    extern void (* WriteVertices )( handle_t self_, const void * src, uintptr_t start, size_t count );

    /// Write index data to a model
    extern void (* WriteIndices )( handle_t self_, const void * src, uintptr_t start, size_t count );

    /// Set the axis-aloigned bounding box of the model
    extern void (* SetBounds )( handle_t self_, const Vector3& bmin, const Vector3& bmax );

    ///Retreive the axis-aligned bounding box of the model
    extern void (* GetBounds )( handle_t self_, Vector3& bmin, Vector3& bmax );
}}

#endif

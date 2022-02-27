
//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODELSTREAM_H__
#define __I3D_MODELSTREAM_H__

#include "i3d/core/Types.h"

namespace i3d {
    
    struct MeshStream {
        uint32_t m_vertexStart;
        uint32_t m_vertexCount;
        uint32_t m_indexStart;
        uint32_t m_indexCount;
    };

    struct ModelStream {
        static const uint32_t VERSION = 5;
        static const uint32_t F_SKINNED = 1;

        uint32_t        m_version;
        uint32_t        m_flags;
        uint32_t        m_vertexCount;
        uint32_t        m_indexCount;
        uint32_t        m_meshCount;
        uint32_t        m_vertexDataSize;
        uint32_t        m_indexDataSize;
        uint32_t        m_offsMeshes;
        uint32_t        m_offsMeshHashArray;
        uint32_t        m_offsMeshHashMap;
        uint32_t        m_offsVertices;
        uint32_t        m_offsIndices;
        uint32_t        m_offsInfluences;
        uint32_t        m_pad;
        float           m_bounds[ 2 ][ 3 ];

        template<typename T>
        const T* GetPointer( uint32_t offs ) const {
            return ( const T * ) ( ( ( uintptr_t ) this ) + offs );
        }

        const MeshStream * GetMeshes() const {
            return GetPointer<MeshStream>( m_offsMeshes );
        }

        const uint32_t * GetIndices() const {
            return GetPointer<uint32_t>( m_offsIndices );
        }

        const void * GetVertices() const {
            return GetPointer<void>( m_offsVertices );
        }
        
        const void * GetInfluences() const {
            return GetPointer<void>( m_offsInfluences );
        }

        const uint64_t * GetMeshHashArray() const {
            return GetPointer<uint64_t>( m_offsMeshHashArray );
        }

        const uint32_t * GetMeshHashMap() const {
            return GetPointer<uint32_t>( m_offsMeshHashMap );
        }
        
        Vector3 GetBoundsMin() const {
            return Vector3( m_bounds[0][0], m_bounds[0][1], m_bounds[0][2]);
        }
        
        Vector3 GetBoundsMax() const {
            return Vector3( m_bounds[1][0], m_bounds[1][1], m_bounds[1][2]);
        }
    };
}

#endif

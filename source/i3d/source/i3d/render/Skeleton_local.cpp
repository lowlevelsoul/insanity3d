//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/Skeleton_local.h"

namespace i3d {
    
    DEFINE_RESOURCE(SkeletonLocal, "skl;" )

    //======================================================================================================================
    SkeletonLocal::SkeletonLocal() {
        m_stream = nullptr;
    }

    //======================================================================================================================
    SkeletonLocal::~SkeletonLocal() {
        if ( m_stream == nullptr ) {
            SAFE_DELETEARRAY( m_stream );
        }
    }
    
    //======================================================================================================================
    void SkeletonLocal::Load( File * file, const char * path ) {
        size_t length = file->Length();
        
        uint8_t * stream = (uint8_t*) new uint8_t[length];  // mem->Allocate( length );
        XE_ASSERT( stream != nullptr );
        
        m_stream = (SkeletonStream *) stream;
        size_t amtRead = file->Read( stream, sizeof(uint8_t), length );
        XE_ASSERT( amtRead == length );
        
        XE_ASSERT( m_stream->m_version == SkeletonStream::VERSION );
    }

    //======================================================================================================================
    size_t SkeletonLocal::GetJointCount() const {
        return m_stream->m_jointCount;
    }

    //======================================================================================================================
    int32_t SkeletonLocal::FindJoint( const char * name ) const {
        
        const uint64_t * hashes = m_stream->GetJointHashArray();
        size_t jointCount = m_stream->m_jointCount;
        
        uint64_t hash = fh64::CalcFromString( name );
        
        int32_t index = -1;
        bool found = Sys::BinarySearch<uint64_t, int32_t>(index, hash, hashes, jointCount );
        if ( found == false ) {
            return -1;
        }
        
        const uint32_t * hashMap = m_stream->GetJointHashMap();
        uint32_t jointIndex = hashMap[ index ];
        
        return (int32_t) jointIndex;
    }
    
    //======================================================================================================================
    int32_t SkeletonLocal::FindJoint( uint64_t nameHash ) const {
        
        const uint64_t * hashes = m_stream->GetJointHashArray();
        size_t jointCount = m_stream->m_jointCount;
        
        int32_t index = -1;
        bool found = Sys::BinarySearch<uint64_t, int32_t>(index, nameHash, hashes, jointCount );
        if ( found == false ) {
            return -1;
        }
        
        const uint32_t * hashMap = m_stream->GetJointHashMap();
        uint32_t jointIndex = hashMap[ index ];
        
        return (int32_t) jointIndex;
    }
    
    //======================================================================================================================
    uint32_t SkeletonLocal::FindJoints( int32_t * indicesOut, const uint64_t * nameHash, size_t count ) const {
        
        uint32_t validCount = 0;
        
        const uint64_t * hashes = m_stream->GetJointHashArray();
        size_t jointCount = m_stream->m_jointCount;
        const uint32_t * hashMap = m_stream->GetJointHashMap();
        
        for ( uint32_t i = 0; i < count; ++i) {
            
            int32_t index = -1;
            bool found = Sys::BinarySearch<uint64_t, int32_t>(index, nameHash[ i ], hashes, jointCount );
            if ( found == false ) {
                indicesOut[ i ] = -1;
            }
            else {
                ++validCount;
                uint32_t jointIndex = hashMap[ index ];
                indicesOut[ i ] = jointIndex;
            }
        }
        
        return validCount;
    }
    
    //======================================================================================================================
    const uint64_t * SkeletonLocal::GetSortedJointNameHashes() const {
        return m_stream->GetJointHashArray();
    }
    
    //======================================================================================================================
    const uint32_t * SkeletonLocal::GetSortedJointNameMap() const {
        return m_stream->GetJointHashMap();
    }

    //======================================================================================================================
    const int32_t * SkeletonLocal::GetJointParents() const {
        return (int32_t *) m_stream->GetJointParents();
    }

    //======================================================================================================================
    const Matrix4 * SkeletonLocal::GetJointLocalMatrices() const {
        return m_stream->GetJointMatrices();
    }

    const Matrix4 * SkeletonLocal::GetJointBindMatrices() const {
        return m_stream->GetJointBindMatrices();
    }
    
    //======================================================================================================================
    const char * SkeletonLocal::GetJointName( uint32_t index ) const {
        return m_stream->GetJointName( index );
    }
    
}

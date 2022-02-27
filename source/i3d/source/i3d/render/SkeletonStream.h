//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SKELETONSTREAM_H__
#define __I3D_SKELETONSTREAM_H__

namespace i3d {

    class SkeletonStream {
    public:
        // Version 1 : Initial version
        // Version 2 : added human readable joint names 
        static const uint32_t VERSION = 2;
        uint32_t        m_version;
        uint32_t        m_flags;
        uint32_t        m_jointCount;
        uint32_t        m_offsJointParents;
        uint32_t        m_offsJointHashArray;
        uint32_t        m_offsJointHashMap;
        uint32_t        m_offsJointMatrices;
        uint32_t        m_offsJointBindMatrices;
        uint32_t        m_offsNameStrings;
        uint32_t        m_offsNames;
        
        template<typename __type__>
        const __type__ * GetPointer( uint32_t offset ) const {
            return (const __type__*) (((uintptr_t) this) + offset);
        }
        
        const int32_t * GetJointParents() const { return GetPointer<int32_t>( m_offsJointParents ); }
        const uint64_t * GetJointHashArray() const { return GetPointer<uint64_t>( m_offsJointHashArray ); }
        const uint32_t * GetJointHashMap() const { return GetPointer<uint32_t>( m_offsJointHashMap ); }
        const Matrix4 * GetJointMatrices() const { return GetPointer<Matrix4>( m_offsJointMatrices ); }
        const Matrix4 * GetJointBindMatrices() const { return GetPointer<Matrix4>( m_offsJointBindMatrices ); }
        const char * GetStrings() const { return GetPointer<char>( m_offsNameStrings ); }
        const uint32_t * GetJointNames() const { return GetPointer<uint32_t>( m_offsNames ); }
        
        const char * GetJointName( uint32_t index ) const {
            assert(index < m_jointCount);
            const char * str = &(GetStrings()[ GetJointNames()[ index ] ]);
            return str;
        }
    };
}

#endif

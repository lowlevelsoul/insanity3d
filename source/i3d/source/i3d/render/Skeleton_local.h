//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SKELETON_LOCAL_H__
#define __I3D_SKELETON_LOCAL_H__

#include "i3d/render/Skeleton.h"
#include "i3d/render/SkeletonStream.h"

namespace i3d {

    class SkeletonLocal : public Skeleton {
    public:
        DECLARE_RESOURCE( SkeletonLocal );
        
        SkeletonLocal();
        
        virtual ~SkeletonLocal();
        
        virtual void Load( File * file, const char * path ) override;
        
        virtual size_t GetJointCount() const override;
        
        virtual int32_t FindJoint( const char * name ) const override;
        
        virtual int32_t FindJoint( uint64_t nameHash ) const override;
        
        virtual uint32_t FindJoints( int32_t * indicesOut, const uint64_t * nameHash, size_t count ) const override;
        
        virtual const int32_t * GetJointParents() const override;
        
        virtual const Matrix4 * GetJointLocalMatrices() const override;
        
        virtual const Matrix4 * GetJointBindMatrices() const override;
        
        virtual const char * GetJointName( uint32_t index ) const override;
        
        virtual const uint64_t * GetSortedJointNameHashes() const override;
        
        virtual const uint32_t * GetSortedJointNameMap() const override;
        
    protected:
        SkeletonStream *    m_stream;
    };
}

#endif

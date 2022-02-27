//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SKELETON_H__
#define __SKELETON_H__

#include "i3d/core/Types.h"
#include "i3d/res/Resource.h"

namespace i3d {
    
    class Skeleton : public Resource {
    public:
        
        virtual ~Skeleton() {
            
        }
        
        virtual size_t GetJointCount() const = 0;
        
        virtual int32_t FindJoint( const char * name ) const = 0;
        
        virtual int32_t FindJoint( uint64_t nameHash ) const = 0;
        
        virtual uint32_t FindJoints( int32_t * indicesOut, const uint64_t * nameHash, size_t count ) const = 0;
        
        virtual const int32_t * GetJointParents() const = 0;
        
        virtual const Matrix4 * GetJointLocalMatrices() const = 0;
        
        virtual const Matrix4 * GetJointBindMatrices() const = 0;
        
        virtual const char * GetJointName( uint32_t index ) const = 0;
        
        virtual const uint64_t * GetSortedJointNameHashes() const = 0;
        
        virtual const uint32_t * GetSortedJointNameMap() const = 0;
    };
}



#endif

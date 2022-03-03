//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_ECSTYPES_H__
#define __I3D_ECSTYPES_H__

#include "i3d/core/Types.h"

namespace i3d {

    static constexpr size_t ECS_MAX_ENTITIES = 2048;
    static constexpr size_t ECS_MAX_COMPONERNTS = 64;
    static constexpr size_t ECS_MAX_SYSTEMS = 128;

    typedef uint64_t Entity;
    typedef uint64_t ComponentType;
    typedef std::bitset<ECS_MAX_COMPONERNTS> EntitySignature;
}


#endif 

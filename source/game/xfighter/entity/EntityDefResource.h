//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENTITYRESOURCE_H__
#define __ENTITYRESOURCE_H__

#include "i3d/rtti/RttiStream.h"

class Entity;

class EntityDefResource : public i3d::RttiResource {
public:
    DECLARE_RESOURCE( EntityDefResource )
    
    EntityDefResource();
    
    virtual ~EntityDefResource();
    
    Entity * Construct();
    
};

#endif


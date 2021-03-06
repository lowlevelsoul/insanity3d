//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENTITYDEF_H__
#define __ENTITYDEF_H__

#include "i3d/rtti/RttiStream.h"

class Entity;

class EntityDef : public i3d::RttiObject {
public:
    RTTI_CLASS_DECLARE( EntityDef, i3d::RttiObject )
    
    EntityDef();
    
    virtual ~EntityDef();
    
    virtual const char * GetTypeName();
    
protected:
};

#endif


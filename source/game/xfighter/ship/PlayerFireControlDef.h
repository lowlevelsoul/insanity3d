//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERFIRECONTROLDEF_H__
#define __PLAYERFIRECONTROLDEF_H__

#include "ship/ShipDef.h"
#include "weapons/ProjectileDef.h"

class PlayerFireControlDef : ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( PlayerFireControlDef, ShipComponentDef )
    
    PlayerFireControlDef();
    
    virtual ~PlayerFireControlDef();
    
    virtual const char * GetTypeName();
    
public:
    typedef i3d::stl::Vector<ProjectileDef::ref_ptr_t> projectiles;
    float                   m_fireInterval;
    projectiles::type       m_projectiles;    
};

#endif

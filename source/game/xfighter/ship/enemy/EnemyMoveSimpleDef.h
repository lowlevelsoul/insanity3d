//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENEMYMOVESIMPLEDEF_H__
#define __ENEMYMOVESIMPLEDEF_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class EnemyMoveSimpleDef : public ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( EnemyMoveSimpleDef, ShipComponentDef );
    
    EnemyMoveSimpleDef();
    
    virtual ~EnemyMoveSimpleDef();
    
    virtual const char * GetTypeName();
    
public:
    i3d::Vector3        m_dir;
    float               m_speed;
};

#endif

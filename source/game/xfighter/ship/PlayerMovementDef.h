//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERMOVEMENTDEF_H__
#define __PLAYERMOVEMENTDEF_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class PlayerMovementDef : public ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( PlayerMovementDef, ShipComponentDef );
    
    PlayerMovementDef();
    
    virtual ~PlayerMovementDef();
    
    virtual const char * GetTypeName();
};

#endif

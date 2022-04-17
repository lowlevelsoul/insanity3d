//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERFIRECONTROL_H__
#define __PLAYERFIRECONTROL_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"
#include "ship/PlayerFireControlDef.h"
#include "weapons/ProjectileDef.h"

class PlayerFireControl : ShipComponent {
public:
    RTTI_CLASS_DECLARE( PlayerFireControl, ShipComponent )
    
    PlayerFireControl();
    
    virtual ~PlayerFireControl();
    
    virtual void Construct( ShipComponentDef * def ) override;
    
    virtual void Think( float timeStep ) override;
    
    void FireProjectiles( );
    
    void FireProjetileAtPos( ProjectileDef * def );
    
public:
    PlayerFireControlDef *      m_def;
    float                       m_fireTimer;
    bool                        m_wasPressed;
};

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPPREVIEW_H__
#define __SHIPPREVIEW_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class EntityDef;

class PlayerMovement : public ShipComponent {
public:
    RTTI_CLASS_DECLARE( PlayerMovement, ShipComponent );
    
    PlayerMovement();
    
    virtual ~PlayerMovement();
    
    virtual void Think( float timeStep ) override;
    
    virtual void Construct( ShipComponentDef * def ) override;
    
    void MoveTouch( float timeStep );
    
    void ClampPlayerPosition();
    
public:
    bool            m_firstFrame;
    float           m_boundsRadius;
    i3d::Vector3    m_boundsCenter;
};

#endif

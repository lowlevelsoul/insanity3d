//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENEMYMOVESIMPLE_H__
#define __ENEMYMOVESIMPLE_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class EntityDef;

class EnemyMoveSimple : public ShipComponent {
public:
    RTTI_CLASS_DECLARE( EnemyMoveSimple, ShipComponent );
    
    EnemyMoveSimple();
    
    virtual ~EnemyMoveSimple();
    
    virtual void Think( float timeStep ) override;
    
    virtual void Construct( ShipComponentDef * def ) override;
    
public:
    i3d::Vector3        m_dir;
    float               m_speed;
};

#endif

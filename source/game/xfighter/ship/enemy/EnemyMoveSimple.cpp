//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/enemy/EnemyMoveSimple.h"
#include "ship/enemy/EnemyMoveSimpleDef.h"
#include "XFighter.h"
#include "misc/GameGlobals.h"

RTTI_CLASS_DEFINE( EnemyMoveSimple )

//======================================================================================================================
EnemyMoveSimple::EnemyMoveSimple() {
    m_dir.Set(0, 0, -1);
    m_speed = 40;
}

//======================================================================================================================
EnemyMoveSimple::~EnemyMoveSimple() {
    
}

//======================================================================================================================
void EnemyMoveSimple::Think( float timeStep ) {
    Ship * ship = GetOwner();
    
    i3d::Vector3 newLocation = ship->GetLocation();
    newLocation += ( m_dir * m_speed * timeStep );
                        
    ship->SetLocation( newLocation );
}

//======================================================================================================================
void EnemyMoveSimple::Construct( ShipComponentDef * def ) {
    EnemyMoveSimpleDef * moveDef = def->SafeCast<EnemyMoveSimpleDef>();
    
    m_dir = moveDef->m_dir;
    m_dir.Normalise();
    
    m_speed = moveDef->m_speed;
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/PlayerGameplay.h"
#include "col/Collider.h"
#include "col/ColTypes.h"

RTTI_CLASS_DEFINE( PlayerGameplay )

//======================================================================================================================
void PlayerGameplay::HitCallback( const OverlapResult & res, Collider * thisObj, Collider * otherObj, void * context ) {
    
    if ( otherObj->m_group & col::GROUP_ENEMY ) {
        
    }
    else if ( otherObj->m_group & col::GROUP_ENEMY_BULLET ) {
        
    }
}

//======================================================================================================================
PlayerGameplay::PlayerGameplay() {
    
}

//======================================================================================================================
PlayerGameplay::~PlayerGameplay() {
    
}

//======================================================================================================================
void PlayerGameplay::Construct( ShipComponentDef * def ) {
    
}

//======================================================================================================================
void PlayerGameplay::Think( float timeStep ) {
    
}


//======================================================================================================================
void PlayerGameplay::HandleEnemyBullet( Projectile * projectile ) {
    
}

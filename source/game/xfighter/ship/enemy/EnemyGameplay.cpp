//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/enemy/EnemyGameplay.h"
#include "col/Collider.h"
#include "col/ColTypes.h"
#include "ship/ColShape.h"
#include "weapons/Projectile.h"
#include "entity/EntityManager.h"

RTTI_CLASS_DEFINE( EnemyGameplay )

//======================================================================================================================
void EnemyGameplay::HitCallback( const OverlapResult & res, Collider * thisObj, Collider * otherObj, void * context ) {
    
    if ( otherObj->m_group & col::GROUP_PLAYER ) {
        
    }
    else if ( otherObj->m_group & col::GROUP_PLAYER_BULLET ) {
        EnemyGameplay * gp = (EnemyGameplay * ) context;
        gp->HandleBullet( (Projectile *) otherObj->m_userData );
    }
}

//======================================================================================================================
EnemyGameplay::EnemyGameplay() {
    m_energy = 100;
    m_damageThisFrame = 0;
}

//======================================================================================================================
EnemyGameplay::~EnemyGameplay() {
    
}

//======================================================================================================================
void EnemyGameplay::Construct( ShipComponentDef * def ) {
    
    ColShape * colShape = GetOwner()->FindComponent<ColShape>();
    if ( colShape != nullptr ) {
        colShape->m_collider.SetHitCallback( HitCallback, this );
    }
}

//======================================================================================================================
void EnemyGameplay::Think( float timeStep ) {
    if ( GetOwner()->IsAlive() == false ) {
        return;
    }
    
    m_energy -= m_damageThisFrame;
    m_damageThisFrame = 0;
    if ( m_energy <= 0 ) {
        entityMgr->RemoveEntity( GetOwner() );
    }
}

//======================================================================================================================
void EnemyGameplay::HandleBullet( Projectile * projectile ) {
    m_damageThisFrame += projectile->m_damage;
    projectile->DestroyFromHit();
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "weapons/Projectile.h"
#include "col/CollideSys.h"
#include "col/Collider.h"
#include "misc/GameGlobals.h"
#include "entity/EntityManager.h"
#include "XFighter.h"

RTTI_CLASS_DEFINE( Projectile )

//======================================================================================================================
Projectile::Projectile() {
    m_direction = i3d::Vector3(0, 0, 1);
    m_speed = 20;
    m_damage = 100;
}

//======================================================================================================================
Projectile::~Projectile() {
    
}

//======================================================================================================================
void Projectile::Construct( EntityDef * def ) {
    
    ProjectileDef * pdef = def->SafeCast<ProjectileDef>();
    if ( pdef->m_isPlayer == true ) {
        m_collider.SetGroup( col::GROUP_PLAYER_BULLET );
        m_collider.SetFilter( col::GROUP_ENEMY );
    }
    else {
        m_collider.SetGroup( col::GROUP_ENEMY_BULLET );
        m_collider.SetFilter( col::GROUP_PLAYER );
    }
    
    m_collider.m_userData = this;

    m_speed = pdef->m_speed;
    m_damage = pdef->m_damage;
}

//======================================================================================================================
void Projectile::OnAdd() {
    colSys->AddShape( &m_collider );
}

//======================================================================================================================
void Projectile::OnDelete() {
    colSys->RemoveShape( &m_collider );
    //XE_LOG("Removing projectile\n");
}

//======================================================================================================================
void Projectile::DestroyFromHit() {
    // It's possible that this is called more than once, so make
    // sure we only process the first hit.
    if ( IsAlive() == false ) {
        return;
    }
    
    entityMgr->RemoveEntity( this );
}

//======================================================================================================================
void Projectile::Think( float timeStep ) {
    
    if ( IsAlive() == false ) {
        return;
    }
    
    bool inPlayfield = IsInPlayfield();
    if ( inPlayfield == false ) {
        entityMgr->RemoveEntity( this );
        return;
    }

    i3d::Vector2 location2( GetLocation().X(), GetLocation().Z() );
    
    m_collider.SetPosition( location2 );
    i3d::Vector3 vel = m_direction * m_speed;
    i3d::Vector3 moveAmt = vel * timeStep;
    m_location += moveAmt;
    
    ProjectileThink( timeStep );
}

//======================================================================================================================
void Projectile::ProjectileThink( float timeStep ) {
    
}

//======================================================================================================================
bool Projectile::IsInPlayfield() const {
    Playfield * playfield = game->m_globals->m_playfield;
    
    float zn, zf;
    playfield->CalcZLimits( zn, zf, 0, GetLocation().X() );
    if ( m_collider.m_boundsWorld[0].Y() > zf || m_collider.m_boundsWorld[1].Y() < zn ) {
        return false;
    }
    
    float xmin, xmax;
    playfield->CalcXLimits( xmin, xmax, 0, m_collider.m_boundsWorld[0].Y()  );
    if ( m_collider.m_boundsWorld[0].X() > xmax || m_collider.m_boundsWorld[1].X() < xmin ) {
        return false;
    }
    
    return true;
}

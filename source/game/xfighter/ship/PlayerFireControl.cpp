//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/PlayerFireControl.h"
#include "ship/PlayerFireControlDef.h"
#include "weapons/ProjectileDef.h"
#include "weapons/Projectile.h"
#include "entity/EntityManager.h"

extern i3d::CVar   in_Player1Fire;

RTTI_CLASS_DEFINE( PlayerFireControl )

//======================================================================================================================
PlayerFireControl::PlayerFireControl() {
    m_wasPressed = false;
    m_fireTimer = 0;
    m_def = nullptr;
}

//======================================================================================================================
PlayerFireControl::~PlayerFireControl() {
    
}

//======================================================================================================================
void PlayerFireControl::Construct( ShipComponentDef * def ) {
    this->m_def = def->SafeCast<PlayerFireControlDef>();
}

//======================================================================================================================
void PlayerFireControl::Think( float timeStep ) {
    bool fire = false;
    
    if ( in_Player1Fire.GetFloat() != 0 ) {
        if ( m_wasPressed == true ) {
            // First press, so we want to fire right away and setup the fire time
            fire = true;
            m_wasPressed = true;
            m_fireTimer = 0;
        }
        else {
            m_fireTimer += timeStep;
            
            if ( m_fireTimer >= m_def->m_fireInterval ) {
                fire = true;
                m_fireTimer -= m_def->m_fireInterval;
            }
        }
    }
    else {
        m_wasPressed = false;
        m_fireTimer = 0;
    }
    
    if ( fire == true ) {
        FireProjectiles();
    }
}

//======================================================================================================================
void PlayerFireControl::FireProjectiles( ) {
    i3d::Vector3 centerPos = GetOwner()->GetLocation();
    //i3d::Vector3 leftPos = centerPos + i3d::Vector3(6, 0, 12);
    //i3d::Vector3 rightPos = centerPos + i3d::Vector3(-6, 0, 12);
    
    ProjectileDef * projDef = m_def->m_projectiles[0];
    
    Projectile * proj = (Projectile*) rtti->Create( projDef->GetTypeName() );
    proj->m_direction = i3d::Vector3(0, 0, 1);
    
    proj->SetLocation( centerPos );
    proj->Construct( projDef );
    
    entityMgr->AddEntity( proj );
}

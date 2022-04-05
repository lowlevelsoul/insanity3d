//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/PlayerMovement.h"
#include "ship/PlayerMovement.h"

#include "XFighter.h"
#include "misc/GameGlobals.h"

RTTI_CLASS_BEGIN(PlayerMovement)
RTTI_CLASS_END(PlayerMovement)

extern i3d::CVar   in_Player1MoveX;
extern i3d::CVar   in_Player1MoveZ;
extern i3d::CVar   in_Player1Fire;
extern i3d::CVar   in_Player2MoveX;
extern i3d::CVar   in_Player2MoveZ;
extern i3d::CVar   in_Player2Fire;
extern i3d::CVar   in_enabled;


//#define XF_USE_TOUCH_INPUT

//======================================================================================================================
PlayerMovement::PlayerMovement() {
    m_boundsRadius = 10.0f;
    m_boundsCenter = i3d::Vector3::ZERO;
    m_firstFrame = true;
}

//======================================================================================================================
PlayerMovement::~PlayerMovement() {
    
}

//======================================================================================================================
void PlayerMovement::Construct( ShipComponentDef * def ) {
}

//======================================================================================================================
void PlayerMovement::Think( float timeStep ) {
    if ( m_firstFrame == true ) {
        float minZ, maxZ;
        game->m_globals->m_playfield->CalcZLimits(minZ, maxZ, 0, 0);
        GetOwner()->SetLocation( i3d::Vector3(0, 0, minZ + 30));
        m_firstFrame = false;
        
        return;
    }
    
#ifdef XF_USE_TOUCH_INPUT
    MoveTouch( timeStep );
#endif
}


//======================================================================================================================
void PlayerMovement::MoveTouch(float timeStep) {
#ifdef XF_USE_TOUCH_INPUT
    if (in_enabled.GetBool() == false) {
        return;
    }

    float moveX = in_Player1MoveX.GetFloat();
    float moveZ = in_Player1MoveZ.GetFloat();
    
#if 0
    m_bankAngle = 0;
    
    if (moveX < 0) {
        m_bankAngle = GameConstants::SHIP_BANK_ANGLE;
    }
    else if (moveX > 0) {
        m_bankAngle = -GameConstants::SHIP_BANK_ANGLE;
    }
#endif
    
    i3d::Vector3 moveVec(moveX, 0, moveZ);
    i3d::Vector3 newLocation = m_owner->GetLocation() + moveVec;
    m_owner->SetLocation( newLocation );
    
    ClampPlayerPosition();
#endif
}

//======================================================================================================================
void PlayerMovement::ClampPlayerPosition() {
    
    i3d::Vector3 location = GetOwner()->GetLocation();

    // Clip the z-position of the ship and apply it.
    float minZ, maxZ;
    game->m_globals->m_playfield->CalcZLimits(minZ, maxZ, 0, location.X());
    
    i3d::Vector3 boundsPos3 = location + m_boundsCenter;
    i3d::Vector2 boundsPos(boundsPos3.X(), boundsPos3.Z());
        
    float zBoundsMin = boundsPos.Y() - m_boundsRadius;
    float zBoundsMax = boundsPos.Y() + m_boundsRadius;
    
    float zDistMax = maxZ - zBoundsMax;
    float zDistMin = minZ - zBoundsMin;
    
    bool locationClamped = false;

    
    if (zDistMax < 0) {
        location += i3d::Vector3(0, 0, zDistMax);
        locationClamped = true;
    }
    else if (zDistMin > 0) {
        location += i3d::Vector3(0, 0, zDistMin);
        locationClamped = true;
    }
    
    // Clip the ship x-position and apply it to the location
    float minX, maxX;
    game->m_globals->m_playfield->CalcXLimits(minX, maxX, 0, location.Z());
    
    boundsPos3 = location + m_boundsCenter;
    boundsPos.Set( boundsPos3.X(), boundsPos3.Z() );
    float xBoundsMin = boundsPos.X() - m_boundsRadius;
    float xBoundsMax = boundsPos.X() + m_boundsRadius;
    
    float xDistMax = maxX - xBoundsMax;
    float xDistMin = minX - xBoundsMin;
    
    if (xDistMax < 0) {
        location += i3d::Vector3(xDistMax, 0, 0);
        locationClamped = true;
    }
    else if (xDistMin > 0) {
        location += i3d::Vector3(xDistMin, 0, 0);
        locationClamped = true;
    }
    
    if ( locationClamped == true ) {
        GetOwner()->SetLocation( location );
    }
}


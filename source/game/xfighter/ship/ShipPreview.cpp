//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ShipPreview.h"
#include "XFighter.h"
#include "misc/GameGlobals.h"

RTTI_CLASS_BEGIN(ShipPreview)
    RTTI_PROP( VEC3,    "location",         m_location )
    RTTI_PROP( VEC3,    "rot_axis",         m_rotationAxis )
    RTTI_PROP( FLOAT,   "rot",              m_rotation )
    RTTI_PROP( FLOAT,   "rot_speed",        m_rotationSpeed )
RTTI_CLASS_END(ShipPreview)

//======================================================================================================================
ShipPreview::ShipPreview() {
    
    m_location      = i3d::Vector3(0, 0, 20);
    m_rotationAxis  = i3d::Vector3::UNIT_Y;;
    m_rotation      = 0;
    m_rotationSpeed = 90;
}

//======================================================================================================================
ShipPreview::~ShipPreview() {
    
}

//======================================================================================================================
void ShipPreview::Think( float timeStep ) {
    float znear, zfar;
    game->m_globals->m_playfield->CalcZLimits( znear, zfar, 0, 0);
    
    //m_rotation += m_rotationSpeed * timeStep;
    while ( m_rotation > 360 ) m_rotation -= 360.0f;
    
    m_location.Set( 0, 0, znear * 0.5f + zfar * 0.5f);
    m_owner->SetLocation( m_location );
    
    i3d::Quaternion rot;
    rot.Set( m_rotationAxis, i3d::scalar::DegToRad( m_rotation ) );
    m_owner->SetRotation( rot );
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ShipPreview.h"
#include "ship/ShipPreviewDef.h"

#include "XFighter.h"
#include "misc/GameGlobals.h"

RTTI_CLASS_BEGIN(ShipPreview)
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
void ShipPreview::Construct( ShipComponentDef * def ) {
    ShipPreviewDef * pdef = def->SafeCast<ShipPreviewDef>();
    m_location          = pdef->m_location;
    m_rotationAxis      = pdef->m_rotationAxis;
    m_rotation          = pdef->m_rotation;
    m_rotationSpeed     = pdef->m_rotationSpeed;
}

//======================================================================================================================
void ShipPreview::Think( float timeStep ) {
    float znear, zfar;
    game->m_globals->m_playfield->CalcZLimits(znear, zfar, 0, 0);

    //m_location = i3d::Vector3(0, 0, znear) + i3d::Vector3(0, 0, 50);
    
    m_rotation += m_rotationSpeed * timeStep;
    while ( m_rotation > 360 ) m_rotation -= 360.0f;
    
    i3d::Quaternion rot;
    rot.Set( m_rotationAxis, i3d::scalar::DegToRad( m_rotation ) );
    
    m_owner->SetLocation( m_location );
    m_owner->SetRotation( rot );
}


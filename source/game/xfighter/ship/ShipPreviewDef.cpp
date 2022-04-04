//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ShipPreviewDef.h"

RTTI_CLASS_BEGIN(ShipPreviewDef)
    RTTI_PROP( VEC3,    "location",         m_location )
    RTTI_PROP( VEC3,    "rot_axis",         m_rotationAxis )
    RTTI_PROP( FLOAT,   "rot",              m_rotation )
    RTTI_PROP( FLOAT,   "rot_speed",        m_rotationSpeed )
RTTI_CLASS_END(ShipPreviewDef)

//======================================================================================================================
ShipPreviewDef::ShipPreviewDef() {
    m_location      = i3d::Vector3(0, 0, 20);
    m_rotationAxis  = i3d::Vector3::UNIT_Y;;
    m_rotation      = 0;
    m_rotationSpeed = 90;
}

//======================================================================================================================
ShipPreviewDef::~ShipPreviewDef() {
    
}

//======================================================================================================================
const char * ShipPreviewDef::GetTypeName() {
    return "ShipPreview";
}

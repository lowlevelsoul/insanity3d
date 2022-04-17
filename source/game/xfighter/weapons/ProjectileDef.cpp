//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "weapons/ProjectileDef.h"

RTTI_CLASS_BEGIN( ProjectileDef )
    RTTI_PROP(  RESOURCE,   "texture",              m_texture )
    RTTI_PROP(  VEC2,       "tex_top_left",         m_texTopLeft )
    RTTI_PROP(  VEC2,       "tex_bottom_right",     m_texBottomRight )
    RTTI_PROP(  BOOL,       "is_player",            m_isPlayer )
    RTTI_PROP(  FLOAT,      "speed",                m_speed )
RTTI_CLASS_END( ProjectileDef )

RTTI_CLASS_BEGIN( ProjectileCircleDef )
    RTTI_PROP( FLOAT,       "radius",               m_radius )
RTTI_CLASS_END( ProjectileCircleDef )

RTTI_CLASS_BEGIN( ProjectileBoxDef )
    RTTI_PROP( FLOAT,       "width",                m_width )
    RTTI_PROP( FLOAT,       "length",               m_length )
RTTI_CLASS_END( ProjectileBoxDef )

//======================================================================================================================
ProjectileDef::ProjectileDef() {
    m_texture = nullptr;
    m_isPlayer = false;
    m_speed = 4;
}

//======================================================================================================================
ProjectileDef::~ProjectileDef() {
    
}


//======================================================================================================================
//======================================================================================================================


//======================================================================================================================
ProjectileCircleDef::ProjectileCircleDef() {
    m_radius = 2;
}

//======================================================================================================================
ProjectileCircleDef::~ProjectileCircleDef() {
    
}

//======================================================================================================================
const char * ProjectileCircleDef::GetTypeName() {
    return "ProjectileCircle";
}

//======================================================================================================================
//======================================================================================================================


//======================================================================================================================
ProjectileBoxDef::ProjectileBoxDef() {
    m_width = 2;
    m_length = 4;
}

//======================================================================================================================
ProjectileBoxDef::~ProjectileBoxDef() {
    
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/PlayerFireControlDef.h"

RTTI_CLASS_BEGIN( PlayerFireControlDef )
    RTTI_PROP( FLOAT,                   "fire_interval",        m_fireInterval )
    RTTI_PROP_ARRAY( OBJECT_REFPTR,     "projectiles",          m_projectiles )
RTTI_CLASS_END( PlayerFireControlDef )

//======================================================================================================================
PlayerFireControlDef::PlayerFireControlDef() {
    m_fireInterval = 0.5f;
}

//======================================================================================================================
PlayerFireControlDef::~PlayerFireControlDef() {
    
}

//======================================================================================================================
const char * PlayerFireControlDef::GetTypeName() {
    return "PlayerFireControl";
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/enemy/EnemyMoveSimpleDef.h"

RTTI_CLASS_BEGIN( EnemyMoveSimpleDef )
    RTTI_PROP( VEC3,            "dir",      m_dir )
    RTTI_PROP( FLOAT,           "speed",      m_speed )
RTTI_CLASS_END( EnemyMoveSimpleDef )

//======================================================================================================================
EnemyMoveSimpleDef::EnemyMoveSimpleDef() {
    m_dir.Set(0, 0, -1);
    m_speed = 40;
}

//======================================================================================================================
EnemyMoveSimpleDef::~EnemyMoveSimpleDef() {
    
}

//======================================================================================================================
const char * EnemyMoveSimpleDef::GetTypeName() {
    return "EnemyMoveSimple";
}

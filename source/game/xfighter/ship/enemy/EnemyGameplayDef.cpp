//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/enemy/EnemyGameplayDef.h"

RTTI_CLASS_DEFINE( EnemyGameplayDef )

//======================================================================================================================
EnemyGameplayDef::EnemyGameplayDef() {
    m_energy = 100;
}

//======================================================================================================================
EnemyGameplayDef::~EnemyGameplayDef() {
    
}

//======================================================================================================================
const char * EnemyGameplayDef::GetTypeName() {
    return "EnemyGameplay";
}

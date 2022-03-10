//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "components/Player.h"

RTTI_CLASS_BEGIN( PlayerPrototype )
    RTTI_PROP( FLOAT,       "energy",               m_energy )
    RTTI_PROP( FLOAT,       "energy_regen",         m_energyRegen )
RTTI_CLASS_END_NAMED( PlayerPrototype, "Player" )

//======================================================================================================================
PlayerPrototype::PlayerPrototype() {
    m_energy = 100;
    m_energyRegen = 150;
}

//======================================================================================================================
PlayerPrototype::~PlayerPrototype() {
}

//======================================================================================================================
void PlayerPrototype::CreateComponent( i3d::Entity ent ) {
    Player * comp = ecs->AddEntityComponent<Player>(ent);
    comp->m_proto = this;
    comp->m_rot = 0;
}

//======================================================================================================================
void PlayerPrototype::Construct( i3d::Entity & ent ) {
}

//======================================================================================================================
void PlayerPrototype::DestroyComponent( i3d::Entity ent ) {
    
}

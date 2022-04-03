//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "entity/Entity.h"


RTTI_CLASS_BEGIN( Entity )
    RTTI_PROP( VEC3,    "location",         m_location )
    RTTI_PROP( QUAT,    "rotation",         m_rotation )
    RTTI_PROP( BOOL,    "do_think",         m_doThink )
    RTTI_PROP( BOOL,    "do_draw",          m_doDraw )
RTTI_CLASS_END( Entity )

//======================================================================================================================
Entity::Entity() : m_deleteNode(this) {
    m_id        = ID_INVALID;
    m_location  = i3d::Vector3::ZERO;
    m_rotation  = i3d::Quaternion::IDENTITY;
    m_doThink = true;
    m_doDraw = true;
}

//======================================================================================================================
Entity::~Entity() {
    
}

//======================================================================================================================
void Entity::Think( float timeStep ) {
    
}

//======================================================================================================================
void Entity::Draw( float timeStep ) {
    
}

//======================================================================================================================
void Entity::OnAdd() {
    
}

//======================================================================================================================
void Entity::OnDelete() {
    
}

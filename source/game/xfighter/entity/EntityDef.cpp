//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "entity/EntityDef.h"
#include "entity/Entity.h"

DEFINE_RESOURCE( EntityDef, "ent;")

//======================================================================================================================
EntityDef::EntityDef() {
    
}

//======================================================================================================================
EntityDef::~EntityDef() {
    
}

//======================================================================================================================
void EntityDef::Load( i3d::File * file, const char * path ) {
    m_stream.ReadFromFile( file );
}

//======================================================================================================================
Entity * EntityDef::Construct() {
    m_stream.Construct();
    i3d::RttiObject * obj = m_stream.m_objects.front();
    
    Entity * ent = obj->SafeCast<Entity>();
    XE_ERROR( ent == nullptr, "First object from rtti stream is not an entity\n");
    
    return ent;
}

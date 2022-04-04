//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "entity/EntityDefResource.h"
#include "entity/Entity.h"
#include "entity/EntityDef.h"

DEFINE_RESOURCE( EntityDefResource, "ent;")

//======================================================================================================================
EntityDefResource::EntityDefResource() {
    
}

//======================================================================================================================
EntityDefResource::~EntityDefResource() {
    
}

//======================================================================================================================
Entity * EntityDefResource::Construct() {
    EntityDef * def = m_object->SafeCast<EntityDef>();
    XE_ERROR(def == nullptr, "Object in entity def resource is not a valid EntityDef\n");
    
    i3d::RttiObject * obj = rtti->Create( def->GetTypeName() );
    Entity * ent = obj->SafeCast<Entity>();
    XE_ERROR( ent == nullptr, "Type %s is not an entity\n", def->GetTypeName() );
    
    ent->Construct( def );
    
    return ent;
}

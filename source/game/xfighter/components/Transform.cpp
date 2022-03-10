//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "components/Transform.h"

RTTI_CLASS_BEGIN( TransformPrototype )
    RTTI_PROP( VEC3, "location", m_location )
    RTTI_PROP( QUAT, "rotation", m_rotation )
RTTI_CLASS_END_NAMED( TransformPrototype, "Transform" )

//======================================================================================================================
TransformPrototype::TransformPrototype() {
    
}

//======================================================================================================================
TransformPrototype::~TransformPrototype() {
}

//======================================================================================================================
void TransformPrototype::CreateComponent( i3d::Entity ent ) {
    ecs->AddEntityComponent<Transform>(ent);
}

//======================================================================================================================
void TransformPrototype::Construct( i3d::Entity & ent ) {
    Transform & comp = ecs->GetComponent<Transform>(ent);
}

//======================================================================================================================
void TransformPrototype::DestroyComponent( i3d::Entity ent ) {
    
}


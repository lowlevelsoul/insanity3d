//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "components/ShipModel.h"
#include "i3d/render/Material.h"

RTTI_CLASS_BEGIN( ShipModelPrototype )
    RTTI_PROP( RESOURCE, "model", m_model )
    RTTI_PROP( RESOURCE, "material", m_materialRes )
RTTI_CLASS_END_NAMED( ShipModelPrototype, "ShipModel" )

//======================================================================================================================
ShipModelPrototype::ShipModelPrototype() {
    
}

//======================================================================================================================
ShipModelPrototype::~ShipModelPrototype() {
}

//======================================================================================================================
void ShipModelPrototype::CreateComponent( i3d::Entity ent ) {
    ecs->AddEntityComponent<ShipModel>(ent);
}

//======================================================================================================================
void ShipModelPrototype::Construct( i3d::Entity & ent ) {
    ShipModel & comp = ecs->GetComponent<ShipModel>(ent);
    comp.m_model = m_model;
    comp.m_material = m_materialRes->GetMaterial();
}

//======================================================================================================================
void ShipModelPrototype::DestroyComponent( i3d::Entity ent ) {
    
}


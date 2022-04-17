//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "weapons/ProjectileCircle.h"

RTTI_CLASS_DEFINE( ProjectileCircle )

//======================================================================================================================
ProjectileCircle::ProjectileCircle() {
    
}

//======================================================================================================================
ProjectileCircle::~ProjectileCircle() {
    
}

//======================================================================================================================
void ProjectileCircle::Construct( EntityDef * def ) {
    ProjectileCircleDef * circleDef = def->SafeCast<ProjectileCircleDef>();
    XE_ERROR(circleDef == nullptr, "Not a valid circle projectile def" );
    
    m_shape.Set( i3d::Vector2::ZERO, circleDef->m_radius );
    m_collider.SetShape( &m_shape );
    
    Super::Construct( def );
}

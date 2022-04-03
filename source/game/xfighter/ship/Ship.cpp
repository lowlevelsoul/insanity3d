//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/Ship.h"

RTTI_CLASS_BEGIN(ShipComponent)
RTTI_CLASS_END(ShipComponent)

RTTI_CLASS_BEGIN(Ship)
        RTTI_PROP_ARRAY( OBJECT_REFPTR,         "components",       m_components )
        RTTI_PROP( RESOURCE,                    "model",            m_model )
        RTTI_PROP( RESOURCE,                    "material",         m_materialMat )
        RTTI_PROP( QUAT,                        "offs_rot",         m_modelOffsRot )
        RTTI_PROP( VEC3,                        "offs_pos",         m_modelOffsPos )
RTTI_CLASS_END(Ship)

//======================================================================================================================
Ship::Ship() {
    m_model = nullptr;
    m_materialMat = nullptr;
    m_modelOffsRot = i3d::Quaternion::IDENTITY;
    m_modelOffsPos = i3d::Vector3::ZERO;
}

//======================================================================================================================
Ship::~Ship() {
    
}

//======================================================================================================================
bool Ship::ReadComplete( const char * path ) {
    for (auto c : m_components ) {
        c->m_owner = this;
    }
    
    return true;
}

//======================================================================================================================
void Ship::Think( float timeStep ) {
    
    i3d::Matrix4 localMod = i3d::Matrix4::IDENTITY;
    localMod.SetRotationQ( m_modelOffsRot );
    localMod.SetTranslation( m_modelOffsPos );
    
    i3d::Matrix4 transform;
    transform.SetRotationQ( m_rotation );
    transform.SetTranslation( m_location );
    
    m_transform.Concat( localMod, transform );
    
    for ( auto c : m_components ) {
        c->Think( timeStep );
    }
}

//======================================================================================================================
void Ship::Draw( float timeStep ) {
    render->SubmitModel( m_model, m_transform, m_materialMat->GetMaterial() );
}

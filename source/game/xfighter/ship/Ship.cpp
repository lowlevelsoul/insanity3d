//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/Ship.h"
#include "ship/ShipDef.h"

RTTI_CLASS_BEGIN(ShipComponent)
RTTI_CLASS_END(ShipComponent)

RTTI_CLASS_BEGIN(Ship)
RTTI_CLASS_END(Ship)

//======================================================================================================================
Ship::Ship() {
    m_model = nullptr;
    m_material = nullptr;
    m_modelOffsRot = i3d::Quaternion::IDENTITY;
    m_modelOffsPos = i3d::Vector3::ZERO;
    m_roll = 0;
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
    
    i3d::Matrix4 roll = i3d::Matrix4::IDENTITY;
    roll.SetRotationAA( i3d::Vector3::UNIT_Z, i3d::scalar::DegToRad( m_roll ) );
    
    i3d::Matrix4 transform;
    transform.SetRotationQ( m_rotation );
    transform.SetTranslation( m_location );
    
    i3d::Matrix4 local;
    local.Concat( localMod, roll);
    m_transform.Concat( local, transform );
    
    for ( auto c : m_components ) {
        c->Think( timeStep );
    }
}

//======================================================================================================================
void Ship::Construct( EntityDef * def ) {
    ShipDef * shipDef = def->SafeCast<ShipDef>();
    XE_ERROR( shipDef == nullptr, "EntityDef is not derived from ShipDef\n");
    
    m_model = shipDef->m_model;
    m_material = shipDef->m_materialMat->GetMaterial();
    m_modelOffsPos = shipDef->m_modelOffsPos;
    m_modelOffsRot = shipDef->m_modelOffsRot;
    
    for (auto c : shipDef->m_components ) {
        
        RttiObject * obj = rtti->Create( c->GetTypeName() );
        ShipComponent * sc = obj->SafeCast<ShipComponent>();
        XE_ERROR( sc == nullptr, "Object %s is not a valid ShipComponent type\n", c->GetTypeName() );
        
        sc->SetOwner( this );
        sc->Construct( c );
        
        m_components.push_back( sc );
    }
}

//======================================================================================================================
void Ship::Draw( float timeStep ) {
    render->SubmitModel( m_model, m_transform, m_material );
}



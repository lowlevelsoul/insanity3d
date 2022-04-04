//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ShipDef.h"
#include "ship/Ship.h"

RTTI_CLASS_BEGIN(ShipComponentDef)
RTTI_CLASS_END(ShipComponentDef)

RTTI_CLASS_BEGIN(ShipDef)
        RTTI_PROP_ARRAY( OBJECT_REFPTR,         "components",       m_components )
        RTTI_PROP( RESOURCE,                    "model",            m_model )
        RTTI_PROP( RESOURCE,                    "material",         m_materialMat )
        RTTI_PROP( QUAT,                        "offs_rot",         m_modelOffsRot )
        RTTI_PROP( VEC3,                        "offs_pos",         m_modelOffsPos )
RTTI_CLASS_END(ShipDef)

//======================================================================================================================
ShipComponentDef::ShipComponentDef() {
    
}

//======================================================================================================================
ShipComponentDef::~ShipComponentDef() {
    
}

//======================================================================================================================
const char * ShipComponentDef::GetTypeName() {
    return "ShipComponent";
}

//======================================================================================================================
ShipDef::ShipDef() {
    m_model = nullptr;
    m_materialMat = nullptr;
    m_modelOffsRot = i3d::Quaternion::IDENTITY;
    m_modelOffsPos = i3d::Vector3::ZERO;
}

//======================================================================================================================
ShipDef::~ShipDef() {
    
}

//======================================================================================================================
const char * ShipDef::GetTypeName() {
    return "Ship";
}

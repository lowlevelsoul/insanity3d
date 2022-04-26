//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ColShapeDef.h"

#ifndef __XFIGHTER_BUILDER__
#   include "col/CollideSys.h"
#endif

RTTI_CLASS_BEGIN( ColShapeDef )
    RTTI_PROP_ARRAY( STRING , "groups", m_groupStrings )
    RTTI_PROP_ARRAY( STRING , "filter", m_filterStrings )
RTTI_CLASS_END( ColShapeDef )

RTTI_CLASS_BEGIN( ColShapeCircleDef )
    RTTI_PROP( VEC2 ,   "center",   m_center )
    RTTI_PROP( FLOAT ,  "radius",   m_radius )
RTTI_CLASS_END( ColShapeCircleDef )

RTTI_CLASS_BEGIN( ColShapeBoxDef )
    RTTI_PROP( VEC2 ,   "size",   m_size )
RTTI_CLASS_END( ColShapeBoxDef )

//======================================================================================================================
ColShapeDef::ColShapeDef() {
    m_group = 0;
    m_filter = 0;
}

//======================================================================================================================
ColShapeDef::~ColShapeDef() {

}

//======================================================================================================================
uint32_t ColShapeDef::BuildMask( const string_vector::type & strings ) {
    uint32_t val = 0;
    
#ifndef __XFIGHTER_BUILDER__
    for (auto & s : strings ) {
        const char * thisStr = s.c_str();
        
        uint32_t thisMask = colSys->FindMask( thisStr );
        XE_ERROR( thisMask == 0, "Invalid collision mask item '%s'", thisStr);
        
        val |= thisMask;
    }
#endif
    
    return val;
}

//======================================================================================================================
bool ColShapeDef::ReadComplete( const char * path ) {
    m_group = BuildMask( m_groupStrings );
    m_filter = BuildMask( m_filterStrings );
    
    return true;
}

//======================================================================================================================
const char * ColShapeDef::GetTypeName() {
    return nullptr;
}


//======================================================================================================================
ColShapeCircleDef::ColShapeCircleDef() {
    m_center = i3d::Vector2::ZERO;
    m_radius = 1;
}

//======================================================================================================================
ColShapeCircleDef::~ColShapeCircleDef() {
    
}

//======================================================================================================================
const char * ColShapeCircleDef::GetTypeName() {
    return "ColShapeCircle";
}


//======================================================================================================================
ColShapeBoxDef::ColShapeBoxDef() {
    m_size.Set(10, 20);
}

//======================================================================================================================
ColShapeBoxDef::~ColShapeBoxDef() {
    
}

//======================================================================================================================
const char * ColShapeBoxDef::GetTypeName() {
    return "ColShapeBox";
}

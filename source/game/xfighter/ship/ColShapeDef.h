//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLSHAPEDEF_H__
#define __COLSHAPEDEF_H__

#include "ship/ShipDef.h"

class ColShapeDef : public ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( ColShapeDef, i3d::RttiObject )
    
    typedef i3d::stl::Vector<i3d::stl::String::type> string_vector;
    
    ColShapeDef();
    
    virtual ~ColShapeDef();
    
    virtual bool ReadComplete( const char * path ) override;
    
    virtual const char * GetTypeName() override;
    
    uint32_t BuildMask( const string_vector::type & strings );

public:    
    string_vector::type             m_groupStrings;
    string_vector::type             m_filterStrings;
    uint32_t                        m_group;
    uint32_t                        m_filter;
};

class ColShapeCircleDef : public ColShapeDef {
public:
    RTTI_CLASS_DECLARE( ColShapeCircleDef, ColShapeDef )
    
    ColShapeCircleDef();
    
    virtual ~ColShapeCircleDef();
    
    virtual const char * GetTypeName() override;
public:
    i3d::Vector2        m_center;
    float               m_radius;
};

#endif

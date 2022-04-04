//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPPREVIEWDEF_H__
#define __SHIPPREVIEWDEF_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class ShipPreviewDef : public ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( ShipPreviewDef, ShipComponentDef );
    
    ShipPreviewDef();
    
    virtual ~ShipPreviewDef();
    
    virtual const char * GetTypeName();
    
public:
    i3d::Vector3        m_location;
    i3d::Vector3        m_rotationAxis;
    float               m_rotation;
    float               m_rotationSpeed;
};

#endif

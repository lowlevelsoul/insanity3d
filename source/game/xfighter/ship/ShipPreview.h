//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPPREVIEW_H__
#define __SHIPPREVIEW_H__

#include "ship/Ship.h"

class ShipPreview : public ShipComponent {
public:
    RTTI_CLASS_DECLARE( ShipPreview, ShipComponent );
    ShipPreview();
    
    virtual ~ShipPreview();
    
    virtual void Think( float timeStep ) override;
    
public:
    i3d::Vector3        m_location;
    i3d::Vector3        m_rotationAxis;
    float               m_rotation;
    float               m_rotationSpeed;
};

#endif

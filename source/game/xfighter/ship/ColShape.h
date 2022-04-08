//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLSHAPE_H__
#define __COLSHAPE_H__

#include "ship/Ship.h"
#include "col/Collider.h"

class ColShapeDef;
class ColMaskDef;

class ColShape : public ShipComponent {
public:
    RTTI_CLASS_DECLARE( ColShape, ShipComponent )
    
    ColShape();
    
    virtual ~ColShape();
    
    virtual void Construct( ShipComponentDef * compDef ) override;
    
    virtual void Think( float timeStep ) override;
    
protected:
    
    uint32_t BuildCollisionMask( ColMaskDef * def );
    
public:
    Collider        m_collider;         ///< The collider used by this shape
};

#endif

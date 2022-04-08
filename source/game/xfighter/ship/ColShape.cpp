//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ship/ColShape.h"
#include "ship/ColShapeDef.h"

#include "col/ColShape.h"
#include "col/ColShapeCircle.h"
#include "col/CollideSys.h"

class ColShapeCircle : public ColShape {
public:
    RTTI_CLASS_DECLARE( ColShapeCircle, ColShape )
    
    ColShapeCircle();
    
    virtual ~ColShapeCircle();
    
    virtual void Construct( ShipComponentDef * compDef ) override;
    
public:
    ShapeCircle     m_circle;
};

RTTI_CLASS_DEFINE( ColShape )

RTTI_CLASS_BEGIN( ColShapeCircle )
RTTI_CLASS_END( ColShapeCircle )

//======================================================================================================================
ColShape::ColShape() {
    
}

//======================================================================================================================
ColShape::~ColShape() {
    
}

//======================================================================================================================
void ColShape::Construct( ShipComponentDef * compDef ) {
    ColShapeDef * shapeDef = compDef->SafeCast<ColShapeDef>();
    
    m_collider.SetGroup( shapeDef->m_group );
    m_collider.SetFilter( shapeDef->m_filter );
    
    // Set position from ship and add to the collision system
    i3d::Vector4 location = GetOwner()->m_transform.m_rows[3];
    
    i3d::Vector2 pos( location.X(), location.Z() );
    m_collider.SetPosition( pos  );
    colSys->AddShape( &m_collider );
}

//======================================================================================================================
void ColShape::Think( float timeStep ) {
    i3d::Vector4 location = GetOwner()->m_transform.m_rows[3];
    i3d::Vector2 pos2( location.X(), location.Z() );
    m_collider.SetPosition( pos2 );
}

//======================================================================================================================
ColShapeCircle::ColShapeCircle() {
    
}

//======================================================================================================================
ColShapeCircle::~ColShapeCircle() {
    
}

//======================================================================================================================
void ColShapeCircle::Construct( ShipComponentDef * compDef ) {
    ColShapeCircleDef * circleDef = compDef->SafeCast<ColShapeCircleDef>();
    
    m_circle.Set( circleDef->m_center, circleDef->m_radius );
    m_collider.SetShape( & m_circle );

    Super::Construct( compDef );
}

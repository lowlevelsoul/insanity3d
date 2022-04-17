//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PROJECTILECIRCLE_H__
#define __PROJECTILECIRCLE_H__

#include "weapons/ProjectileDef.h"
#include "weapons/Projectile.h"
#include "col/ColShapeCircle.h"

class ProjectileCircle : public Projectile {
public:
    RTTI_CLASS_DECLARE( ProjectileCircle, Projectile )
    
    ProjectileCircle();
    
    virtual ~ProjectileCircle();
    
    virtual void Construct( EntityDef * def ) override;
    
public:
    ShapeCircle     m_shape;
};

#endif

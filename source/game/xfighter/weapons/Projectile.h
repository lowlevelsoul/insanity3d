//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity/Entity.h"
#include "weapons/ProjectileDef.h"
#include "col/Collider.h"
#include "col/ColShapeBox.h"
#include "col/ColShapeCircle.h"

class Projectile : public Entity {
public:
    RTTI_CLASS_DECLARE( Projectile, Entity )
    
    Projectile();
    
    virtual ~Projectile();
    
    virtual void Construct( EntityDef * def ) override;
    
    virtual void Think( float timeStep ) override;
    
    virtual void ProjectileThink( float timeStep );
    
    virtual void OnAdd() override;
    
    virtual void OnDelete() override;
    
    bool IsInPlayfield() const;
    
public:
    i3d::Texture *          m_texture;
    Collider                m_collider;
    i3d::Vector3            m_direction;
    float                   m_speed;
};

#endif

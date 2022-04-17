//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PROJETILEDEF_H__
#define __PROJETILEDEF_H__

#include "entity/EntityDef.h"

class ProjectileDef : public EntityDef {
public:
    RTTI_CLASS_DECLARE( ProjectileDef, EntityDef )
    
    ProjectileDef();
    
    virtual ~ProjectileDef();
    
public:
    // Graphical stuff
    i3d::Texture *      m_texture;              ///< Texture we're using
    i3d::Vector2        m_texTopLeft;           ///<
    i3d::Vector2        m_texBottomRight;       ///<
                                                ///
    // General stuff
    bool                m_isPlayer;             ///< If true, this is a projectile that belongs to the player
    float               m_speed;                ///< Speed that the projectile is travelling
};

class ProjectileCircleDef : public ProjectileDef {
public:
    RTTI_CLASS_DECLARE( ProjectileCircleDef, ProjectileDef )
    
    ProjectileCircleDef();
    
    virtual ~ProjectileCircleDef();
    
    virtual const char * GetTypeName() override;
    
public:
    float       m_radius;           /// Circle radius
};

class ProjectileBoxDef : public ProjectileDef {
public:
    RTTI_CLASS_DECLARE( ProjectileBoxDef, ProjectileDef )
    
    ProjectileBoxDef();
    
    virtual ~ProjectileBoxDef();
    
public:
    float       m_width;
    float       m_length;
};

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYER_H__
#define __PLAYER_H__

class PlayerPrototype;

struct Player {
    PlayerPrototype *       m_proto;
    float                   m_rot;
};

class PlayerPrototype : public i3d::ComponentPrototype {
public:
    RTTI_CLASS_DECLARE_NAMED( PlayerPrototype, i3d::ComponentPrototype )
    
    PlayerPrototype();
    
    virtual ~PlayerPrototype();
    
    virtual void CreateComponent( i3d::Entity ent );
    
    virtual void Construct( i3d::Entity & ent );
    
    virtual void DestroyComponent( i3d::Entity ent );
    
public:
    float       m_energy;
    float       m_energyRegen;
};

#endif

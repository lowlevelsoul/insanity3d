//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_H__
#define __XFIGHTER_H__

#include "i3d/i3d.h"
#include "components/Transform.h"
#include "components/ShipModel.h"
#include "components/Player.h"
#include "systems/TransformManager.h"
#include "systems/ShipManager.h"
#include "systems/PlayerManager.h"

namespace i3d {
    class Material;
    class MaterialResource;
    class Model;
    class PrototypeResource;
}

class XFighter : public i3d::Game {
public:
    XFighter();
    
    virtual ~XFighter();
    
    virtual void Initialise() override;
    
    virtual void Finalise() override;
    
    virtual void Think( float deltaTime ) override;
    
    virtual void Draw( float deltaTime ) override;
    
protected:
    
    void InitialiseEcs();
    
public:
    i3d::MaterialResource *     m_materialRes;
    i3d::Model *                m_model;
    i3d::PrototypeResource *    m_player;
    float                       m_rot;
    
    i3d::ComponentArray<Transform, i3d::ECS_MAX_ENTITIES>   m_transforms;
    i3d::ComponentArray<ShipModel, 128>                     m_shipModels;
    i3d::ComponentArray<Player, 2>                          m_players;
    
    TransformManager            m_transformManager;
    ShipManager                 m_shipManager;
    PlayerManager               m_playerManager;
};

#endif

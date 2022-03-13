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
#include "components/Camera.h"
#include "systems/TransformManager.h"
#include "systems/ShipManager.h"
#include "systems/PlayerManager.h"
#include "systems/SceneManager.h"

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
    
    virtual void Think( float deltaTime, uint32_t viewWidth, uint32_t viewHeight ) override;
    
    virtual void Draw( float deltaTime, uint32_t viewWidth, uint32_t viewHeight ) override;
    
protected:
    
    void CreateSystemEnt();
    
    void InitialiseEcs();
    
public:
    i3d::MaterialResource *     m_materialRes;
    i3d::Model *                m_model;
    i3d::PrototypeResource *    m_player;
    float                       m_rot;
    
    i3d::Entity                 m_systemEnt;            ///< System entoty that holds all of the singletons and thigns
    
    i3d::ComponentArray<Transform, i3d::ECS_MAX_ENTITIES>   m_transforms;
    i3d::ComponentArray<ShipModel, 128>                     m_shipModels;
    i3d::ComponentArray<Player, 2>                          m_players;
    i3d::ComponentSingleton<Camera>                         m_camera;
    
    TransformManager            m_transformManager;
    ShipManager                 m_shipManager;
    PlayerManager               m_playerManager;
    SceneManager                m_sceneManager;
};

#endif

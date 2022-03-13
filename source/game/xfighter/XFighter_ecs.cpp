//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "components/Transform.h"
#include "components/ShipModel.h"
#include "components/Player.h"

#include "systems/TransformManager.h"


//======================================================================================================================
void XFighter::InitialiseEcs() {
    ecs->PublishComponent<Transform>( &m_transforms );
    ecs->PublishComponent<ShipModel>( &m_shipModels );
    ecs->PublishComponent<Player>( &m_players );
    ecs->PublishComponent<Camera>( &m_camera );
    
    ecs->PublishSystem<Transform>( &m_transformManager );
    ecs->PublishSystem<ShipModel, Transform>( &m_shipManager );
    ecs->PublishSystem<Player, ShipModel, Transform>( &m_playerManager );
    ecs->PublishSystem<Camera>( &m_sceneManager );
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "PlayerManager.h"
#include "components/Transform.h"
#include "components/Player.h"
#include "components/ShipModel.h"

#include "i3d/render/Render.h"

//======================================================================================================================
PlayerManager::PlayerManager() {
    
}

//======================================================================================================================
PlayerManager::~PlayerManager() {
    
}

//======================================================================================================================
void PlayerManager::Think( float timeStep ) {
    
    for( auto e : m_entities ) {
        Player & player = ecs->GetComponent<Player>(e);
        Transform &  tf = ecs->GetComponent<Transform>(e);
        
        player.m_rot += 180 * timeStep;
        while (player.m_rot >= 360.0f) {
            player.m_rot -= 360.0f;
        }
        
        tf.m_location.Set(0, 0, 20);
        tf.m_rotation.Set( i3d::Vector3::UNIT_Y, i3d::scalar::DegToRad( player.m_rot ) );
    }
}

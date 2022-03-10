//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ShipManager.h"
#include "components/Transform.h"
#include "components/ShipModel.h"
#include "i3d/render/Render.h"

//======================================================================================================================
ShipManager::ShipManager() {
    
}

//======================================================================================================================
ShipManager::~ShipManager() {
    
}

//======================================================================================================================
void ShipManager::Draw( float timeStep ) {
    
    for( auto e : m_entities ) {
        Transform & tf = ecs->GetComponent<Transform>(e);
        ShipModel & mdl = ecs->GetComponent<ShipModel>(e);
        
        render->SubmitModel( mdl.m_model, tf.m_transform, mdl.m_material );
    }
}

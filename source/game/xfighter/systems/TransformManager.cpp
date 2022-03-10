//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "TransformManager.h"
#include "components/Transform.h"

//======================================================================================================================
TransformManager::TransformManager() {
    
}

//======================================================================================================================
TransformManager::~TransformManager() {
    
}

//======================================================================================================================
void TransformManager::Think( float timeStep ) {
    
    for( auto e : m_entities ) {
        Transform & tf = ecs->GetComponent<Transform>(e);
        
        tf.m_transform.SetRotationQ( tf.m_rotation );
        tf.m_transform.SetTranslation( tf.m_location );
    }    
}

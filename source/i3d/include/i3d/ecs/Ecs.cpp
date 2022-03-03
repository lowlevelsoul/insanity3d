//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/ecs/Ecs.h"

i3d::EcsSystem * ecs = nullptr;

namespace i3d {
    
    //======================================================================================================================
    void EcsCreate() {
        if ( ecs == nullptr ) {
            ecs = new EcsSystem;
        }
    }

    //======================================================================================================================
    void EcsDestroy() {
        SAFE_DELETE( ecs );
    }


    //======================================================================================================================
    EcsSystem::EcsSystem() {
        
    }
    
    //======================================================================================================================
    EcsSystem::~EcsSystem() {
        
    }
    
    //======================================================================================================================
    void EcsSystem::FreeEntity( Entity ent ) {
        const EntitySignature & sig = m_entities.GetSignature( ent );
        m_components.RemoveEntity( ent, sig );
        m_entities.FreeEntity( ent );
    }
    
    //======================================================================================================================
    void EcsSystem::PublishSystem( System * system, const EntitySignature & sig ) {
        m_systems.PublishSystem( system, sig );
    }
}

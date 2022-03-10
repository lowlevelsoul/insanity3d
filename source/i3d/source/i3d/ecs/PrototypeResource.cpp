//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/ecs/PrototypeResource.h"
#include "i3d/ecs/EntityPrototype.h"

namespace i3d {
    
    DEFINE_RESOURCE( PrototypeResource, "proto;" )
    
    //======================================================================================================================
    PrototypeResource::PrototypeResource() {
        m_object = nullptr;
    }
    
    //======================================================================================================================
    PrototypeResource::~PrototypeResource() {
        SAFE_DELETE( m_object );
    }
    
    //======================================================================================================================
    Entity PrototypeResource::CreateEntity() {
        EntityPrototype * proto = m_object->SafeCast<EntityPrototype>();
        XE_ASSERT( proto != nullptr );        
        return proto->Create();
    }
}

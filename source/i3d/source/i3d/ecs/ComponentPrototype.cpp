//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/ecs/ComponentPrototype.h"

namespace i3d {

    //======================================================================================================================
    ComponentPrototype::ComponentPrototype() {
        
    }

    //======================================================================================================================
    ComponentPrototype::~ComponentPrototype() {
        
    }

    //======================================================================================================================
    void ComponentPrototype::CreateComponent( Entity ent ) {
        // Empty - does nothing
    }
    
    //======================================================================================================================
    void ComponentPrototype::Construct( Entity & ent ) {
        // Empty - does nothing
    }
    
    //======================================================================================================================
    void ComponentPrototype::DestroyComponent( Entity ent ) {
        // Empty - does nothing
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/ecs/EntityPrototype.h"
#include "i3d/ecs/Ecs.h"


namespace i3d {
    
    RTTI_CLASS_BEGIN( EntityPrototype )
        RTTI_PROP_ARRAY( OBJECT_PTR,    "components",   m_components )
    RTTI_CLASS_END_NAMED( EntityPrototype, "Entity" )
    
    //======================================================================================================================
    EntityPrototype::EntityPrototype() {
        
    }

    //======================================================================================================================
    EntityPrototype::~EntityPrototype() {
        
    }

    //======================================================================================================================
    Entity EntityPrototype::Create() {
        
        Entity ent = ecs->AllocEntity();
        
        for ( auto * c : m_components ) {
            
            c->CreateComponent( ent );
            
            if ( c->WerePropsRead() == true ) {
                c->Construct( ent );
            }
        }
        
        return ent;
    }
}

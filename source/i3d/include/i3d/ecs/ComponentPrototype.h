//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_COMPONENTPROTOTYPE_H__
#define __I3D_COMPONENTPROTOTYPE_H__

#include "i3d/rtti/Rtti.h"
#include "i3d/ecs/EcsTypes.h"

namespace i3d {
    
    class ComponentPrototype : public RttiObject {
    public:        
        ComponentPrototype();
        
        virtual ~ComponentPrototype();
        
        virtual void CreateComponent( Entity ent );
        
        virtual void Construct( Entity & ent );
        
        virtual void DestroyComponent( Entity ent );
    };
}


#endif

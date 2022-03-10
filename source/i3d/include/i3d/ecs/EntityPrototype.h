//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_ENTITYPROTOTYPE_H__
#define __I3D_ENTITYPROTOTYPE_H__

#include "i3d/rtti/Rtti.h"
#include "i3d/ecs/EcsTypes.h"
#include "i3d/ecs/ComponentPrototype.h"

namespace i3d {
    
    class EntityPrototype : public RttiObject {
    public:
        RTTI_CLASS_DECLARE_NAMED(EntityPrototype, RttiObject )
        
        EntityPrototype();
        
        virtual ~EntityPrototype();
        
        Entity Create();
        
    protected:
        stl::Vector<ComponentPrototype*>::type      m_components;
    };
}


#endif

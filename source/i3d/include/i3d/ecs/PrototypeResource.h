//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_PROTOTYPERESOURCE_H__
#define __I3D_PROTOTYPERESOURCE_H__

#include "i3d/rtti/RttiObject.h"
#include "i3d/rtti/RttiScriptResource.h"
#include "i3d/ecs/EcsTypes.h"

namespace i3d {

    class PrototypeResource : public RttiScriptResource {
    public:
        DECLARE_RESOURCE( PrototypeResource )
        
        PrototypeResource();
        
        virtual ~PrototypeResource();

        Entity CreateEntity();
    };
}

#endif


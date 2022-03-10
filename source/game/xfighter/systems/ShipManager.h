//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPMANAGER_H__
#define __SHIPMANAGER_H__

#include "i3d/ecs/System.h"

class ShipManager : public i3d::System {
public:
    ShipManager();
    
    ~ShipManager();
    
    void Draw( float timeStep );
    
protected:
    
};


#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "i3d/ecs/System.h"

class PlayerManager : public i3d::System {
public:
    PlayerManager();
    
    ~PlayerManager();
    
    void Think( float timeStep );
    
protected:
    
};


#endif

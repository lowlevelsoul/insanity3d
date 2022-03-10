//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TRANSFORMMANAGER_H__
#define __TRANSFORMMANAGER_H__

#include "i3d/ecs/System.h"

class TransformManager : public i3d::System {
public:
    TransformManager();
    
    ~TransformManager();
    
    void Think( float timeStep );
    
protected:
    
};


#endif

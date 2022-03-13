//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "i3d/ecs/System.h"

class SceneManager : public i3d::System {
public:
    SceneManager();
    
    ~SceneManager();
    
    void BeginScene( float timeStep, uint32_t viewWidth, uint32_t viewHeight );
    
    void EndScene();
    
protected:
    
};


#endif

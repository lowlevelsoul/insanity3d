//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GAMEGLOBALS_H__
#define __GAMEGLOBALS_H__

#include "misc/Camera.h"
#include "misc/Playfield.h"

class GameGlobals : public i3d::RttiObject {
public:
    RTTI_CLASS_DECLARE( GameGlobals, i3d::RttiObject )
    
    GameGlobals();
    
    virtual ~ GameGlobals();
    
    void Think( float timeStep, int width, int height );
    
public:
    Camera::ref_ptr_t         m_camera;
    Playfield::ref_ptr_t      m_playfield;
};

#endif

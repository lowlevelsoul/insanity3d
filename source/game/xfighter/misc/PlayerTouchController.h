//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERTOUCHCONTROLLER_H__
#define __PLAYERTOUCHCONTROLLER_H__

class PlayerTouchController {
public:
    i3d::Vector3        m_lastTouchPos;         ///< The past position of the touch event on the playfield reference plane
    i3d::Vector3        m_moveDelta;
    bool                m_touching;
    
    PlayerTouchController();
    
    ~PlayerTouchController();

    void Begin(float screenX, float screenY);

    void Moved(float screenX, float screenY);

    void End(float screenX, float screenY);
    
    bool GetPlayfieldPickPoint( i3d::Vector3& point, float screenX, float screenY);
    
    void CalcPickRay( i3d::Vector3& start, i3d::Vector3& dir, float screenX, float screenY);
    
    i3d::Vector3 CalcPickRayDirWorld(float screenX, float screenY);
};

#endif

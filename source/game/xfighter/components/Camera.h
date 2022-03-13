//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __CAMERA_H__
#define __CAMERA_H__

struct Camera {
    enum MODE {
        MODE_YPR=0,
        MODE_TARGET
    };
    
    MODE        m_mode;
    float       m_fovDegrees;
    float       m_nearClip;
    float       m_farClip;
    i3d::Vector3     m_eye;
    i3d::Vector3     m_target;
    float       m_pitch;
    float       m_yaw;
    float       m_roll;
};

class CameraPrototype : public i3d::ComponentPrototype {
public:
    RTTI_CLASS_DECLARE( CameraPrototype, i3d::ComponentPrototype )
    
    CameraPrototype();
    
    virtual ~CameraPrototype();
    
    virtual void CreateComponent( i3d::Entity ent );
    
    virtual void Construct( i3d::Entity & ent );
    
    virtual void DestroyComponent( i3d::Entity ent );
    
public:
    float           m_fovDegrees;
    float           m_nearClip;
    float           m_farClip;
    i3d::Vector3    m_eye;
    float           m_pitch;
    float           m_yaw;
    float           m_roll;
};

#endif

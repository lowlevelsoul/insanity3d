//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera : public i3d::RttiObject {
public:
    RTTI_CLASS_DECLARE( Camera, i3d::RttiObject )
    
    Camera();
    
    virtual ~Camera();
    
    void Update( int32_t width, int32_t height );
    
public:
    i3d::Vector3        m_location;             ///< Location of the camera in world-space
    i3d::Quaternion     m_rotation;             ///< Rotatton of the camera in world-space
    i3d::Matrix4        m_transform;            ///< Camera world-space transform
    i3d::Matrix4        m_transformInv;         ///< Camera inverse world-space transform
    i3d::Matrix4        m_projection;           ///< Camera projection matrix
    float               m_pitch;
    float               m_yaw;
    float               m_fov;                  ///< Vertical field-of-view in degrees
    float               m_aspect;               ///< Aspect ration of the camera
    float               m_clipNear;             ///< Distance to the near clip-plane
    float               m_clipFar;              ///< Distance to the far clip-plane
    
    i3d::Frustum        m_frustum;              ///< Camera view frustum
};

#endif

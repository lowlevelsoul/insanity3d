//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "misc/Camera.h"

RTTI_CLASS_BEGIN( Camera )
    RTTI_PROP(      VEC3,       "location",             m_location )
    RTTI_PROP(      VEC3,       "rotation",             m_rotation )
    RTTI_PROP(      FLOAT,      "fov_degrees",          m_fov )
    RTTI_PROP(      FLOAT,      "clip_near",            m_clipNear )
    RTTI_PROP(      FLOAT,      "clip_far",             m_clipFar )
    RTTI_PROP(      FLOAT,      "pitch_degrees",        m_pitch )
    RTTI_PROP(      FLOAT,      "yaw_degrees",          m_yaw )
RTTI_CLASS_END( Camera )

//======================================================================================================================
Camera::Camera() {
    m_location = i3d::Vector3::ZERO;
    m_rotation = i3d::Quaternion::IDENTITY;
    m_transform = i3d::Matrix4::IDENTITY;
    m_transformInv  = i3d::Matrix4::IDENTITY;
    m_projection  = i3d::Matrix4::IDENTITY;
    m_fov = 50;
    m_aspect = 16.0f / 9.0f;
    m_clipNear = 5;
    m_clipFar = 500;
    m_pitch = 70;
    m_yaw = 0;

}

//======================================================================================================================
Camera::~Camera() {
    
}

//======================================================================================================================
void Camera::Update( int32_t width, int32_t height ) {
    m_aspect = (float) width / (float) height;
    
    i3d::Quaternion qPitch, qYaw;
    qPitch.Set( i3d::Vector3::UNIT_X, i3d::scalar::DegToRad( m_pitch) );
    qYaw.Set( i3d::Vector3::UNIT_Y, i3d::scalar::DegToRad( m_yaw) );
    m_rotation.Concat( qPitch, qYaw );
    
    // Calc world and innverse world transform
    m_transform.SetRotationQ(m_rotation);
    m_transform.SetTranslation( m_location );
    
    m_transformInv.Inverse(m_transform);
    
    // Calc the projection matrix
    float fovRad = i3d::scalar::DegToRad(m_fov);
    m_projection.SetProjectionPerspLH(fovRad, m_aspect, m_clipNear, m_clipFar);
}


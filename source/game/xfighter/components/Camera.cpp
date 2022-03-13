//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "components/Camera.h"

RTTI_CLASS_BEGIN( CameraPrototype )
    RTTI_PROP( FLOAT,   "fov_degrees",      m_fovDegrees    )
    RTTI_PROP( FLOAT,   "clip_near",        m_nearClip      )
    RTTI_PROP( FLOAT,   "clip_far",         m_farClip       )
    RTTI_PROP( VEC3,    "eye",              m_eye           )
    RTTI_PROP( FLOAT,   "pitch",            m_pitch         )
    RTTI_PROP( FLOAT,   "yaw",              m_yaw           )
    RTTI_PROP( FLOAT,   "roll",             m_roll          )
RTTI_CLASS_END_NAMED( CameraPrototype, "Camera" )

//======================================================================================================================
CameraPrototype::CameraPrototype() {
    m_fovDegrees = 80;
    m_nearClip = 5;
    m_farClip = 500;
    m_eye = i3d::Vector3::ZERO;
    m_pitch = 0;
    m_yaw = 0;
    m_roll = 0;
}

//======================================================================================================================
CameraPrototype::~CameraPrototype() {
    
}

//======================================================================================================================
void CameraPrototype::CreateComponent( i3d::Entity ent ) {
    ecs->AddEntityComponent<Camera>(ent);
}

//======================================================================================================================
void CameraPrototype::Construct( i3d::Entity & ent ) {
    Camera & comp = ecs->GetComponent<Camera>(ent);
    
    comp.m_mode         = Camera::MODE_YPR;
    comp.m_fovDegrees   = m_fovDegrees;
    comp.m_nearClip     = m_nearClip;
    comp.m_farClip      = m_farClip;
    comp.m_eye          = m_eye;
    comp.m_target       = i3d::Vector3::ZERO;
    comp.m_pitch        = m_pitch;
    comp.m_yaw          = m_yaw;
    comp.m_roll         = m_roll;
}

//======================================================================================================================
void CameraPrototype::DestroyComponent( i3d::Entity ent ) {
    
}

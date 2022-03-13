//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "systems/SceneManager.h"
#include "components/Camera.h"
#include "i3d/render/Render.h"

//======================================================================================================================
SceneManager::SceneManager() {
    
}

//======================================================================================================================
SceneManager::~SceneManager() {
    
}

//======================================================================================================================
void SceneManager::BeginScene( float timeStep, uint32_t viewWidth, uint32_t viewHeight ) {
    bool gotCamera = false;
    Camera camera;
    
    for ( auto e : m_entities ) {
        if ( gotCamera == false ) {
            Camera & cam = ecs->GetComponent<Camera>(e);
            camera = cam;
            gotCamera = true;
        }
    }
    
    float aspect = ( (float) viewWidth / (float) viewHeight );
    
    i3d::Matrix4 projMat, viewMat;
    projMat.SetProjectionPerspLH( i3d::scalar::DegToRad( camera.m_fovDegrees ), aspect, camera.m_nearClip, camera.m_farClip );
    
    viewMat = i3d::Matrix4::IDENTITY;
    viewMat.SetTranslation( camera.m_eye );
    viewMat.SetRotationAA( i3d::Vector3::UNIT_X, i3d::scalar::DegToRad( camera.m_pitch ) );
    
    int32_t viewport[] = { 0, 0, (int32_t) viewWidth - 1 , (int32_t) viewHeight - 1 };
    render->BeginScene3d( projMat, viewMat, viewport );
}

//======================================================================================================================
void SceneManager::EndScene() {
    render->EndScene3d();
}

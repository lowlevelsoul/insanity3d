//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "misc/Camera.h"
#include "misc/GameGlobals.h"
#include "misc/PlayerTouchController.h"

//======================================================================================================================
PlayerTouchController::PlayerTouchController() {
    m_touching = false;
}

//======================================================================================================================
PlayerTouchController::~PlayerTouchController() {
    
}

//======================================================================================================================
void PlayerTouchController::Begin( float screenX, float screenY ) {
    bool hit = GetPlayfieldPickPoint(m_lastTouchPos, screenX, screenY);
    assert(hit == true);
    m_touching = true;
}

//======================================================================================================================
void PlayerTouchController::Moved( float screenX, float screenY ) {
    i3d::Vector3 touchPos;
    bool hit = GetPlayfieldPickPoint(touchPos, screenX, screenY);
    assert(hit == true);
    
    m_moveDelta = touchPos - m_lastTouchPos;
    m_lastTouchPos = touchPos;
}

//======================================================================================================================
void PlayerTouchController::End(float screenX, float screenY) {
    m_touching = false;
}

//======================================================================================================================
bool PlayerTouchController::GetPlayfieldPickPoint( i3d::Vector3& point, float screenX, float screenY) {
    i3d::Plane refPlane;
    refPlane.Set(i3d::Vector3::UNIT_Y, 0);
    
    i3d::Vector3 pickLineStart, pickLineDir;
    CalcPickRay(pickLineStart, pickLineDir, screenX, screenY);
    
    i3d::Vector3 pickLineEnd = pickLineStart + pickLineDir * 2000.0f;

    float t;
    i3d::Plane::TEST_RESULT hit = refPlane.TestSegment(point, t, pickLineStart, pickLineEnd);
    return hit == i3d::Plane::TEST_RESULT_HIT;
}

//======================================================================================================================
void PlayerTouchController::CalcPickRay( i3d::Vector3 & start, i3d::Vector3 & dir, float screenX, float screenY) {
    start = game->m_globals->m_camera->m_location;
    dir = CalcPickRayDirWorld(screenX, screenY);
}

//======================================================================================================================
i3d::Vector3 PlayerTouchController::CalcPickRayDirWorld( float screenX, float screenY ) {
    float invViewWidth = 1.0f / (float) game->m_viewWidth;
    float invViewHeight = 1.0f / (float) game->m_viewHeight;
    float clipX = (2.0f * screenX * invViewWidth) - 1.0f;
    float clipY = (2.0f * -screenY * invViewHeight) + 1.0f;
    
    i3d::Vector4 clipCoords(clipX, clipY, 0, 1);
    
    i3d::Matrix4 invProjection;
    invProjection.Inverse( game->m_globals->m_camera->m_projection );
    
    i3d::Vector4 rayDirView = invProjection * clipCoords;
    rayDirView.Z() = 1;
    rayDirView.W() = 0;
    
    i3d::Vector4 rayDirWorld = game->m_globals->m_camera->m_transform * rayDirView;
    i3d::Vector3 result = rayDirWorld;
    result.Normalise(result);
    
    return result;
}


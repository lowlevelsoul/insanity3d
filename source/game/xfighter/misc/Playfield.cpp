//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "misc/Playfield.h"
#include "misc/Camera.h"

RTTI_CLASS_BEGIN( Playfield )
    RTTI_PROP(      FLOAT,           "play_height",        m_playHeight )
RTTI_CLASS_END( Playfield )

Playfield* playfield = nullptr;

//======================================================================================================================
Playfield::Playfield() {
    assert(playfield == nullptr);
    playfield = this;
    
    m_cameraGroundPointValid = false;
    m_playHeight = 0;
}

//======================================================================================================================
Playfield::~Playfield() {
    assert(playfield != nullptr);
    playfield = this;
}

//======================================================================================================================
void Playfield::Update( Camera * camera ) {
    
    m_frustum.Set(camera->m_fov, camera->m_aspect,
                             camera->m_clipNear, camera->m_clipFar,
                             camera->m_transform, true);
    
    m_cameraTransform = camera->m_transform;
    m_cameraNear = camera->m_clipNear;
    m_cameraFar = camera->m_clipFar;
    
    CalcGroundPoint();
    
    // Calc playfield bounds
    float minZ, maxZ;
    CalcZLimits(minZ, maxZ, m_playHeight, 0);
    
    float minX, maxX;
    CalcXLimits(minX, maxX, m_playHeight, maxZ);
    
    m_boundsMin.Set(minX, minZ);
    m_boundsMax.Set(maxX, maxZ);
    
    float minXNear, maxXNear;
    CalcXLimits(minXNear, maxXNear, m_playHeight, minZ);
    
    m_limitsXNear.Set(minXNear, maxXNear);
    m_limitsXFar.Set(minX, maxX);
}

//======================================================================================================================
void Playfield::CalcGroundPoint() {
    
    i3d::Plane basePlane( i3d::Vector3::UNIT_Y, m_playHeight );
    
    i3d::Vector3 cameraDir = m_cameraTransform.m_rows[2];
    i3d::Vector3 cameraStartP = m_cameraTransform.m_rows[3];
    cameraStartP += cameraDir * m_cameraNear;
    
    i3d::Vector3 cameraEndP = m_cameraTransform.m_rows[3];
    cameraEndP += cameraDir * m_cameraFar;
    
    i3d::Vector3 groundPoint;
    float groundT;
    i3d::Plane::TEST_RESULT hitGround = basePlane.TestSegment(groundPoint, groundT, cameraStartP, cameraEndP);
    
    m_cameraGroundPoint = groundPoint;
    m_cameraGroundPointValid = ( hitGround == i3d::Plane::TEST_RESULT_HIT );
}

//======================================================================================================================
void Playfield::CalcZLimits(float & znear, float & zfar, float height, float xpos) {
    assert(m_cameraGroundPointValid == true);
    
    i3d::Vector3 znearTestPoint = m_cameraGroundPoint + i3d::Vector3(xpos, height, -2000);
    i3d::Vector3 zfarTestPoint = m_cameraGroundPoint + i3d::Vector3(xpos, height, 2000);
    
    i3d::Vector3 znearPoint, zfarPoint;
    
    bool hitNearZ = m_frustum.ClipSegment(znearPoint, m_cameraGroundPoint, znearTestPoint);
    bool hitFarZ = m_frustum.ClipSegment(zfarPoint, m_cameraGroundPoint, zfarTestPoint);
    //XE_UNUSED(hitNearZ);
    //XE_UNUSED(hitFarZ);
    
    znear = znearPoint.Z();
    zfar = zfarPoint.Z();
}

//======================================================================================================================
void Playfield::CalcXLimits( float & left, float & right, float height, float zpos ) {
    i3d::Vector3 rightTestPoint(50000, height, zpos);
    i3d::Vector3 leftTestPoint(-50000, height, zpos);
    i3d::Vector3 origin(0, height, zpos);
    
    i3d::Vector3 rightPoint, leftPoint;
    
    bool rightHit = m_frustum.ClipSegment( rightPoint, origin, rightTestPoint );
    bool leftHit = m_frustum.ClipSegment( leftPoint, origin, leftTestPoint );
    //XE_UNUSED(rightHit);
    //XE_UNUSED(leftHit);
    
    left = leftPoint.X();
    right = rightPoint.X();
}

//======================================================================================================================
void Playfield::CalcNormalisedToPlayfieldPoint( i3d::Vector2& p, const i3d::Vector2& np ) {
    float pXT = (1.0f + np.X()) / 2.0f;
    float pZT = (1.0f + np.Y()) / 2.0f;
    
    float pY = m_boundsMin.Y() * (1.0f - pZT) + m_boundsMax.Y() * pZT;
    
    float right = m_limitsXNear.Y() * (1.0f - pZT) + m_limitsXFar.Y() * pZT;
    float left = -right;
    float pX = (left * (1.0f - pXT)) + (right * pXT);
    
    p.Set(pX, pY);
}

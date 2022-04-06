//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/ColShape.h"
#include "col/ColShapeCircle.h"
#include "col/Collider.h"

//======================================================================================================================
ShapeCircle::ShapeCircle() {
    m_type = TYPE_CIRCLE;
    m_radiusSqr = 1;
}

//======================================================================================================================
ShapeCircle::~ShapeCircle() {
    
}

//======================================================================================================================
void ShapeCircle::Set(const i3d::Vector2& center, float radius) {
    m_center = center;
    m_radiusSqr = radius * radius;
    UpdateLocalBounds();
}

//======================================================================================================================
void ShapeCircle::SetCenter(const i3d::Vector2& center) {
    m_center = center;
    UpdateLocalBounds();
}

//======================================================================================================================
void ShapeCircle::SetRadius(float radius) {
    m_radiusSqr = radius * radius;
    UpdateLocalBounds();
}

//======================================================================================================================
void ShapeCircle::UpdateBounds() {
    assert(m_collider != nullptr);
    m_centerWorld = m_center + m_collider->m_pos;
}

//======================================================================================================================
void ShapeCircle::UpdateLocalBounds() {
    float halfRad = std::sqrtf(m_radiusSqr) / 2.0f;
    i3d::Vector2 half(halfRad, halfRad);
    
    m_min = m_center - halfRad;
    m_max = m_center + halfRad;
    
    // Shape has changed, so we need to update it in the collision system
    if (m_collider) {
        m_collider->MarkDiry();
    }
}

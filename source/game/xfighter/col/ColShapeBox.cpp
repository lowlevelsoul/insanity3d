//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/ColShapeBox.h"
#include "col/Collider.h"

//======================================================================================================================
ShapeBox::ShapeBox() {
    m_type = Shape::TYPE_AABOX;
    m_width = 0;
    m_height = 0;
}

//======================================================================================================================
ShapeBox::~ShapeBox() {
    
}

//======================================================================================================================
void ShapeBox::Set(float width, float height) {
    m_width = width;
    m_height = height;
    UpdateLocalBounds();
}

//======================================================================================================================
void ShapeBox::SetWidth(float width) {
    m_width = width;
    UpdateLocalBounds();
}

//======================================================================================================================
void ShapeBox::SetHeight(float height) {
    m_height = height;
    UpdateLocalBounds();
}
//======================================================================================================================
void ShapeBox::UpdateLocalBounds() {
    i3d::Vector2 half(m_width / 2.0f, m_height / 2.0f);
    m_min = -half;
    m_max = half;
    
    // Box size has changed, so we need to update it in the collision system
    if (m_collider) {
        m_collider->MarkDiry();
    }
}

//======================================================================================================================
void ShapeBox::UpdateBounds() {
    assert(m_collider != nullptr);
    
    m_minWorld = m_min + m_collider->m_pos;
    m_maxWorld = m_max + m_collider->m_pos;
}

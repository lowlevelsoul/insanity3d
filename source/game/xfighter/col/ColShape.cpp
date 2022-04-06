//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/CollideSys.h"
#include "col/ColShape.h"
#include "col/ColQuadTree.h"

//======================================================================================================================
Shape::Shape() {
    m_type = TYPE_NONE;
    m_collider = nullptr;
    m_min = i3d::Vector2::ZERO;
    m_max = i3d::Vector2::ZERO;
}

//======================================================================================================================
Shape::~Shape() {
    
}

//======================================================================================================================
void Shape::UpdateBounds() {
    // Empty - does nothing
}

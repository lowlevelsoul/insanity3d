//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/CollideSys.h"
#include "col/Collider.h"
#include "col/ColQuadTree.h"
#include "col/ColShape.h"

//======================================================================================================================
Collider::Collider() : m_listNode(this), m_cellListNode(this) {
    m_flags = 0;
    m_group = 0;
    m_filter = 0;
    m_cell = nullptr;
    m_pos.Set(0, 0);
    m_rot = 0;
    m_hitCallback =  nullptr;
    m_hitCallbackContext = nullptr;
    m_shape = nullptr;
    m_label = "none";
    m_dirtyIndex = -1;
    
    m_radius = 0;
    m_boundsLocal[0] = i3d::Vector2::ZERO;
    m_boundsLocal[1] = i3d::Vector2::ZERO;
    m_boundsWorld[0] = i3d::Vector2::ZERO;
    m_boundsWorld[1] = i3d::Vector2::ZERO;
}

//======================================================================================================================
Collider::~Collider() {
    
}

//======================================================================================================================
void Collider::SetRotation( float rot ) {
    m_rot = rot;
    MarkDiry();
}

//======================================================================================================================
void Collider::SetPosition(const i3d::Vector2& pos) {
    m_pos = pos;    
    MarkDiry();
}

//======================================================================================================================
void Collider::MarkDiry() {
    if (IsAdded() && IsDirty() == false) {
        colSys->AddDirty(this);
    }
}

//======================================================================================================================
void Collider::RemoveFromCell() {
    if (m_cell != nullptr) {
        m_cellListNode.Remove();
        m_cell = nullptr;
    }
}

//======================================================================================================================
void Collider::AddToCell(Cell* newCell) {
    if ((m_cell != nullptr) && (newCell != m_cell)) {
        RemoveFromCell();
    }
    
    if (newCell != m_cell) {
        newCell->m_objects.InsertAfter( &m_cellListNode );
        m_cell = newCell;
    }
}

//======================================================================================================================
void Collider::SetShape(Shape* shape) {
    if (m_shape != nullptr) {
        m_shape->m_collider = nullptr;
        m_shape = nullptr;
    }
    
    if (shape != nullptr) {
        m_shape = shape;
        m_shape->m_collider = this;
    }
}

//======================================================================================================================
void Collider::UpdateBounds() {
    
    // Update the rotation
    m_basis.SetRotation( m_rot );
    
    if (m_shape != nullptr) {
        m_shape->UpdateBounds();
        
        m_boundsLocal[0] = m_shape->m_min;
        m_boundsLocal[1] = m_shape->m_max;
        m_boundsWorld[0] = m_shape->m_min + m_pos;
        m_boundsWorld[1] = m_shape->m_max + m_pos;
        
        i3d::Vector2 size = m_boundsLocal[1] - m_boundsLocal[0];
        m_radius = size.Magnitude() / 2.0f;
    }
}

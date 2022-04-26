//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "col/ColTypes.h"
#include "i3d/core/IntrusiveList.h"
    
class Cell;
class Shape;
class Collider;
struct OverlapResult;

typedef void (*HitCallback)(const OverlapResult & res, Collider * thisObj, Collider * otherObj, void* context);

/// An object in the collision system. A holds all of the basic information that the collision systems requires for an object
/// that we want to use for collisions. The only thing the collider does not have, is information regarding its shape. The
/// actual shape of the collider is defined by a Shape derived object that is attached to the collider.
class Collider {
public:
    typedef i3d::IntrusiveList<Collider> ListNode;
    
    static const uint32_t F_DIRTY = 1;
    
    ListNode                m_listNode;         ///< The list node used link this shape to the collision system
    ListNode                m_cellListNode;     ///< The list node used to link this shape within a cell

    uint32_t                m_flags;            ///< Collision flags
    uint32_t                m_group;            ///< Bit field to decribe the collision types that this shape belongs to
    uint32_t                m_filter;           ///< Bit field to describe the collition types that this shape can collide with
    int32_t                 m_dirtyIndex;       ///< Index of the collider in the dity list
    
    const char *            m_label;            ///< Debug label for the shape
    Shape *                 m_shape;            ///< Pointer to the shape attached to this collider
    
    i3d::Vector2            m_pos;              ///< Object position
    float                   m_rot;              ///< Object rotation in radians
    i3d::Matrix2            m_basis;            ///< Object rotation as a 2x2 matrix
    i3d::Vector2            m_boundsLocal[2];   ///< Local bounds of the object
    i3d::Vector2            m_boundsWorld[2];   ///< World space bounds of the object
    float                   m_radius;           ///< Bounding radius
    
    Cell*                   m_cell;             ///< The Cell the this shape is wholly contained within
    void *                  m_userData;         ///< Pointer to the user data
    HitCallback             m_hitCallback;
    void *                  m_hitCallbackContext;

    Collider();
    
    ~Collider();
    
    void SetLabel(const char* label) { m_label = label; }
    
    void SetDirty() { m_flags |= F_DIRTY; }
    
    void ClearDirty() { m_flags &= ~F_DIRTY; m_dirtyIndex = -1; }
    
    bool IsDirty() const { return (m_flags & F_DIRTY) != 0; }
    
    void SetShape(Shape* shape);
    
    Shape* GetShape();
    
    const Shape* GetShape() const;
    
    void SetPosition(const i3d::Vector2& pos);
    
    void SetRotation( float rot );
    
    void SetFilter(uint32_t filter) { m_filter = filter; }
    
    void SetGroup(uint32_t group) { m_group = group; }
    
    /// Marks the collider as being "dity" in the collision system
    void MarkDiry();
    
    /// Remove this collider from a collision cell
    void RemoveFromCell();
    
    /// Add this collider to a collision cell
    void AddToCell(Cell* cell);
    
    /// Perform the collision callback for this collider
    void DoHitCallback(OverlapResult& res, Collider* otherObj);
    
    /// Sets the callback function and its context data to signal when a collision has occured
    void SetHitCallback(HitCallback callback, void* context);
    
    /// Tell the caller if this collider has been added to the collision system
    bool IsAdded() const { return m_listNode.IsRoot() == false; }
    
    /// Tells us if this collider is contained within a collision cell
    bool IsInCell() const { return m_cellListNode.IsRoot() == false; }
    
    /// Get the local bounds of the collider
    void GetLocalBounds( i3d::Vector2& bmin, i3d::Vector2& bmax );
    
    /// Get the world bounds of the colider
    void GetWorldBounds( i3d::Vector2& bmin, i3d::Vector2& bmax );
    
    /// Update the world bounds for the collider.
    ///@todo This is an awful method name. Change it to something more accurate.
    void UpdateBounds();
};

//======================================================================================================================
inline Shape* Collider::GetShape() {
    return m_shape;
}

//======================================================================================================================
inline const Shape* Collider::GetShape() const {
    return m_shape;
}

//======================================================================================================================
inline void Collider::DoHitCallback(OverlapResult& res, Collider* otherObj) {
    if (m_hitCallback != nullptr) {
        m_hitCallback(res, this, otherObj, m_hitCallbackContext);
    }
}

//======================================================================================================================
inline void Collider::SetHitCallback(HitCallback callback, void* context) {
    m_hitCallback = callback;
    m_hitCallbackContext = context;
}

//======================================================================================================================
inline void Collider::GetLocalBounds( i3d::Vector2& bmin, i3d::Vector2& bmax) {
    bmin = m_boundsLocal[0];
    bmax = m_boundsLocal[1];
}

//======================================================================================================================
inline void Collider::GetWorldBounds( i3d::Vector2& bmin, i3d::Vector2& bmax) {
    bmin = m_boundsWorld[0];
    bmax = m_boundsWorld[1];
}

#endif

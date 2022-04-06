//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLSHAPE_H__
#define __COLSHAPE_H__

#include "col/ColTypes.h"
#include "i3d/core/IntrusiveList.h"

class Collider;
    
/// The base class for sny shape that can be used in the collision system
class Shape {
    friend class Collider;
public:
    typedef i3d::IntrusiveList<Shape> ListNode;
    
    /// The types of shape that are supported in the collisions system
    enum TYPE {
        TYPE_NONE=0,
        TYPE_CIRCLE,
        TYPE_AABOX,
        TYPE_COUNT,
        TYPE_FORCE32 = 0xffffffff
    };

    TYPE                m_type;             ///< Shape type
    Collider*           m_collider;         ///< Pointer to the collider that this shape is attached to
    i3d::Vector2        m_min;              ///< Shape local bounds
    i3d::Vector2        m_max;              ///< Shape local bounds
    
    Shape();
    
    virtual ~Shape();
    
    /// Updates the world bounds of a shape. Called by the collider the shape is attached to when it must
    /// update its world bounds state in order to determine where to attached the collider in the collision tree
    virtual void UpdateBounds();
    
    /// Get a pointer to the collider this shape is attached to
    ///@returns Pointer to the collider
    Collider* GetCollider() { return m_collider; }

    /// Get a pointer to the collider this shape is attached to
    ///@returns Const ointer to the collider
    const Collider* GetCollider() const { return m_collider; }
};

#endif 

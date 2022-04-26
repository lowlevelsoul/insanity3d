//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLSHAPEBOX_H__
#define __COLSHAPEBOX_H__

#include "col/ColShape.h"

/// Defines an axis-aligned box shape. The Axis aligned box does NOT inherit the rotartion of the collider that it is
/// attached to.
class ShapeBox : public Shape {
protected:
    float               m_width;        ///< Width of the box
    float               m_height;       ///< height of the box
    i3d::Vector2        m_minWorld;     ///< Min bounds of the box in world space
    i3d::Vector2        m_maxWorld;     ///< Max bounds of the box in world space
public:
    
    /// Constructor
    ShapeBox();
    
    /// Destructor
    virtual ~ShapeBox();
    
    ///@copydoc Shape
    virtual void UpdateBounds() override;
    
    /// Sets the size of the box. The box is assumed to be positioned locally half-way between
    /// width and height.
    ///@note The internal world bonds of the box will be recalculated when this is called
    ///@param width The width of the box
    ///@param height The height of the box
    void Set(float width, float height);

    /// Sets the width of the box
    ///@note The internal world bonds of the box will be recalculated when this is called
    ///@param width The width of the box
    ///@see Set
    void SetWidth(float width);

    /// Sets the height of the box
    ///@note The internal world bonds of the box will be recalculated when this is called
    ///@param height The height of the box
    ///@see Set
    void SetHeight(float height);
    
    /// Gets the width of the box
    float GetWidth() const { return m_width; }
    
    /// Gets the height of the box
    float GetHeight() const { return m_height; }
    
    const i3d::Vector2& GetMinWorld() const;
    
    const i3d::Vector2& GetMaxWorld() const;
    
protected:
    
    /// Internal method for updating the local world bounds of the box
    void UpdateLocalBounds();
};

//======================================================================================================================
inline const i3d::Vector2& ShapeBox::GetMinWorld() const {
    return m_minWorld;
}

//======================================================================================================================
inline const i3d::Vector2& ShapeBox::GetMaxWorld() const {
    return m_maxWorld;
}

#endif

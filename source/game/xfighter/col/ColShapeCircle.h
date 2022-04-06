//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_COLSHAPECIRCLE_H__
#define __XFIGHTER_COLSHAPECIRCLE_H__

#include "col/ColShape.h"

class ShapeCircle : public Shape {
protected:
    i3d::Vector2        m_centerWorld;      ///< Worldspace center
    i3d::Vector2        m_center;           ///< Local offset of the circle from the collider
    float               m_radiusSqr;        ///< Squared radius of the circle
public:
    ShapeCircle();
    
    virtual ~ShapeCircle();
    
    virtual void UpdateBounds() override;
    
    void Set(const i3d::Vector2& center, float radius);
    
    void SetCenter(const i3d::Vector2& center);
    
    void SetRadius(float radius);
    
    float GetRadiusSqr() const;
    
    const i3d::Vector2& GetCenter() const;
    
    const i3d::Vector2& GetCenterWorld() const;
    
protected:
    void UpdateLocalBounds();
};

//======================================================================================================================
inline float ShapeCircle::GetRadiusSqr() const {
    return m_radiusSqr;
}

//======================================================================================================================
inline const i3d::Vector2& ShapeCircle::GetCenter() const {
    return m_center;
}

//======================================================================================================================
inline const i3d::Vector2& ShapeCircle::GetCenterWorld() const {
    return m_centerWorld;
}

#endif

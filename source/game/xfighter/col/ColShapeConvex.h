//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLSHAPEHULL_H__
#define __COLSHAPEHULL_H__

#include "col/ColShape.h"

class ShapeHull : public Shape {
public:
    ShapeHull();
    
    virtual ~ShapeHull();
    
    ///@copydoc Shape
    virtual void UpdateBounds() override;
    
    void Set( const i3d::Vector2 * verts, size_t pointCount );
    
    void Set( const i3d::Vector2 & bmin, const i3d::Vector2 & bmax );
    
    void CalcNormals( size_t pointCount );
    
public:
    typedef i3d::stl::Vector<i3d::Vector2> vec2_array;
    typedef i3d::stl::Vector<float> float_array;
    
    vec2_array::type          m_points;               ///< Local-space points for the hull
    vec2_array::type          m_normals;              ///< Local-space normals for the hull
    vec2_array::type          m_pointsWorld;          ///< World-space points, updated from the collider the shape is attached to
    vec2_array::type          m_normalsWorld;         ///< World-space normals, updated from the collider the shape is attached to
    float_array::type         m_normalDistancesWorld;
};

#endif

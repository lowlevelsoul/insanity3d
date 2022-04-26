//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/ColShape.h"
#include "col/Collider.h"
#include "col/ColShapeConvex.h"


//======================================================================================================================
ShapeHull::ShapeHull() {
    
}

//======================================================================================================================
ShapeHull::~ShapeHull() {
    
}

//======================================================================================================================
void ShapeHull::UpdateBounds() {
    
    // Transform points into world space
    for ( uint32_t i = 0; i < m_points.size(); ++i) {
        m_pointsWorld[ i ] = ( m_collider->m_basis * m_points[ i ] ) + m_collider->m_pos;
    }
    
    // Transform normals into world space
    // Note: do this seperate from verts as we may have a differing number of normals
    //       for some convex shapes.
    for ( uint32_t i = 0; i < m_normals.size(); ++i ) {
        m_normalsWorld[ i ] = ( m_collider->m_basis * m_normals[ i ] );
    }
    
    
}

//======================================================================================================================
void ShapeHull::Set( const i3d::Vector2 & bmin, const i3d::Vector2 & bmax ) {
    i3d::Vector2 points[4] = {
        bmin,
        i3d::Vector2( bmax.X(), bmin.Y() ),
        bmax,
        i3d::Vector2( bmin.X(), bmax.Y() )
    };
    
    m_points.resize( 4 );
    m_pointsWorld.resize( 4 );
    
    const i3d::Vector2 * currPoint = points;
    for ( auto & p : m_points ) {
        p = *currPoint;
        ++currPoint;
    }
    
    m_normals.resize( 2 );
    m_normalsWorld.resize( 2 );
    
    m_normals[0].Set(0, 1);
    m_normals[1].Set(1, 0);
    
    m_min = bmin;
    m_max = bmax;
}

//======================================================================================================================
void ShapeHull::Set( const i3d::Vector2 * points, size_t pointCount ) {
    // Copy over the points
    m_points.resize( pointCount );
    m_pointsWorld.resize( pointCount );
    
    const i3d::Vector2 * currPoint = points;
    for ( auto & p : m_points ) {
        p = *currPoint;
        ++currPoint;
    }
    
    // Calculate the local bounds of the shape
    m_min = points[0];
    m_max = m_min;
    for ( auto & p : m_points ) {
        m_min.Min(p, m_min);
        m_max.Max(p, m_max);
    }
    
    CalcNormals( pointCount );
}

//======================================================================================================================
void ShapeHull::CalcNormals( size_t pointCount ) {
    
    m_normals.resize( pointCount );
    m_normalsWorld.resize( pointCount );
    
    uint32_t icurr = 0;
    uint32_t inext = 1;
    
    for ( auto & n : m_normals ) {
        i3d::Vector2 e = m_points[ inext ] - m_points[ icurr ];
        i3d::Vector2 ln( -e.Y(), -e.X() );
        
        ln.Normalise(ln);
        n = ln;
        
        icurr = inext;
        inext = ( inext + 1 ) % pointCount;
    }
}

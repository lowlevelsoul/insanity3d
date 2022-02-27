//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_VECTOR2_H__
#define __I3D_VECTOR2_H__

#include "i3d/math/Scalar.h"

namespace i3d {
    class Vector2 {
    public:
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 ZERO;
    
        Vector2() {
            // Empty - does nothing
        }
    
        Vector2( scalar_t x, scalar_t y ) {
            m_x = x;
            m_y = y;
        }
    
        scalar_t X() const { return m_x; }
        scalar_t Y() const { return m_y; }
    
        scalar_t & X() { return m_x; }
        scalar_t & Y() { return m_y; }
    
        void Set( scalar_t x, scalar_t y ) {
            m_x = x;
            m_y = y;
        }
    
        void Set( const Vector2 & rhs ) {
            m_x = rhs.m_x;
            m_y = rhs.m_y;
        }
        
        Vector2 & operator=( const Vector2 & rhs ) {
            Set( rhs );
            return *this;
        }
        
    protected:
        float       m_x;
        float       m_y;
    };
}

#endif

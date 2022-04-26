//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MATRIX2_H__
#define __I3D_MATRIX2_H__

#include "i3d/math/Scalar.h"
#include "i3d/math/Vector2.h"

namespace i3d {

    class Matrix2 {
    public:
        static const Matrix2 IDENTITY;
        
        Matrix2();
        
        Matrix2( const Matrix2 & rhs );
        
        Matrix2( const Vector2 & r0, const Vector2 & r1 );
        
        Matrix2( float x, float y );
        
        void Concat( const Matrix2 & lhs, const Matrix2 & rhs );
        
        void Transform( Vector2 & dst, const Vector2 & src ) const;
        
        void SetRotation( float angleRadians );
        
        Matrix2 & operator=( const Matrix2 & rhs );
        
        Matrix2 operator*(const Matrix2 & rhs ) const;
        
        Vector2 operator*(const Vector2 & rhs ) const;
        
    public:
        Vector2         m_rows[2];
    };
    
    //======================================================================================================================
    inline Matrix2::Matrix2() {
        // Empty - does nothing
    }
    
    //======================================================================================================================
    inline Matrix2::Matrix2( const Matrix2 & rhs ) {
        *this = rhs;
    }
    
    //======================================================================================================================
    inline Matrix2::Matrix2( const Vector2 & r0, const Vector2 & r1 ) {
        m_rows[0] = r0;
        m_rows[1] = r1;
    }
    
    //======================================================================================================================
    inline Matrix2::Matrix2( float x, float y ) {
        m_rows[0].Set( x, 0 );
        m_rows[1].Set( 0, y );
    }
    
    //======================================================================================================================
    inline void Matrix2::Transform( Vector2 & dst, const Vector2 & src ) const {
        dst = m_rows[0] * src.X() +
              m_rows[1] * src.Y();
    }
    
    //======================================================================================================================
    inline void Matrix2::Concat( const Matrix2 & lhs, const Matrix2 & rhs ) {
        rhs.Transform( m_rows[0], lhs.m_rows[0] );
        rhs.Transform( m_rows[1], lhs.m_rows[1] );
    }
    
    //======================================================================================================================
    inline void Matrix2::SetRotation( float angleRadians ) {
        float c = scalar::Cos( angleRadians );
        float s = scalar::Sin( angleRadians );
        
        m_rows[0].Set( c, -s );
        m_rows[1].Set( s, c );
    }
    
    //======================================================================================================================
    inline Matrix2 & Matrix2::operator=( const Matrix2 & rhs ) {
        m_rows[0] = rhs.m_rows[0];
        m_rows[1] = rhs.m_rows[1];
        return *this;
    }
    
    //======================================================================================================================
    inline Matrix2 Matrix2::operator*(const Matrix2 & rhs ) const {
        Matrix2 res = *this;
        res.Concat(*this, rhs);
        return res;
    }
    
    //======================================================================================================================
    inline Vector2 Matrix2::operator*( const Vector2 & rhs ) const {
        Vector2 tmp;
        Transform(tmp, rhs);
        return tmp;
    }
}

#endif

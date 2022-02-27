//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/math/Math3d.h"

namespace i3d {

    #define SIGN(VAL) ((VAL) < 0) ? -1.0f : 1.0f;

    #define calcCofactor3x3(a, b, c, d) ((a * d) - (b * c))

    /*
        a b c
        d e f
        g h i
    */
    #define calcCofactor(a, b, c, d, e, f, g, h, i)\
        ((a * calcCofactor3x3(e, f, h, i)) -\
        (b * calcCofactor3x3(d, f, g, i)) +\
        (c * calcCofactor3x3(d, e, g, h)))

    #define a0 rhs.m_rows[ 0 ].X()
    #define a1 rhs.m_rows[ 0 ].Y()
    #define a2 rhs.m_rows[ 0 ].Z()
    #define a3 rhs.m_rows[ 0 ].W()

    #define a4 rhs.m_rows[ 1 ].X()
    #define a5 rhs.m_rows[ 1 ].Y()
    #define a6 rhs.m_rows[ 1 ].Z()
    #define a7 rhs.m_rows[ 1 ].W()

    #define a8  rhs.m_rows[ 2 ].X()
    #define a9  rhs.m_rows[ 2 ].Y()
    #define a10 rhs.m_rows[ 2 ].Z()
    #define a11 rhs.m_rows[ 2 ].W()

    #define a12 rhs.m_rows[ 3 ].X()
    #define a13 rhs.m_rows[ 3 ].Y()
    #define a14 rhs.m_rows[ 3 ].Z()
    #define a15 rhs.m_rows[ 3 ].W()

    /*
            a0  a1  a2  a3
            a4  a5  a6  a7
            a8  a9 a10 a11
            a12 a13 a14 a15
    */

    //======================================================================================================================
    void Matrix4::Inverse( const Matrix4& rhs ) {
        // Calculate the first 4 cofactors so we can calculate the determinate and early out
        // if it is too small
        scalar_t c0 = calcCofactor( a5, a6, a7, a9, a10, a11, a13, a14, a15 );
        scalar_t c1 = calcCofactor( a4, a6, a7, a8, a10, a11, a12, a14, a15 );
        scalar_t c2 = calcCofactor( a4, a5, a7, a8, a9, a11, a12, a13, a15 );
        scalar_t c3 = calcCofactor( a4, a5, a6, a8, a9, a10, a12, a13, a14 );

        scalar_t det = (a0 * c0) - (a1 * c1) + (a2 * c2) - (a3 * c3);
        if ( std::abs( det ) < 0.6e-5f ) {
            *this = Matrix4::IDENTITY;
            return;
        }

        scalar_t a = 1.0f / det;

        // If we get to here, we have a large enough determinate that we can use to calculate the inverse
        // so we need to calculate the rest of the coefficients
        scalar_t c4 = calcCofactor( a1, a2, a3, a9, a10, a11, a13, a14, a15 );
        scalar_t c5 = calcCofactor( a0, a2, a3, a8, a10, a11, a12, a14, a15 );
        scalar_t c6 = calcCofactor( a0, a1, a3, a8, a9, a11, a12, a13, a15 );
        scalar_t c7 = calcCofactor( a0, a1, a2, a8, a9, a10, a12, a13, a14 );

        scalar_t c8 = calcCofactor( a1, a2, a3, a5, a6, a7, a13, a14, a15 );
        scalar_t c9 = calcCofactor( a0, a2, a3, a4, a6, a7, a12, a14, a15 );
        scalar_t c10 = calcCofactor( a0, a1, a3, a4, a5, a7, a12, a13, a15 );
        scalar_t c11 = calcCofactor( a0, a1, a2, a4, a5, a6, a12, a13, a14 );

        scalar_t c12 = calcCofactor( a1, a2, a3, a5, a6, a7, a9, a10, a11 );
        scalar_t c13 = calcCofactor( a0, a2, a3, a4, a6, a7, a8, a10, a11 );
        scalar_t c14 = calcCofactor( a0, a1, a3, a4, a6, a7, a8, a9, a11 );
        scalar_t c15 = calcCofactor( a0, a1, a2, a4, a5, a6, a8, a9, a10 );

        // Create the adjunct matrix as four vectors
        Vector4 adjRight    ( c0,-c4, c8,-c12 );
        Vector4 adjUp       (-c1, c5,-c9, c13 );
        Vector4 adjAt       ( c2,-c6, c10,-c14 );
        Vector4 adjPos      (-c3, c7,-c11, c15 );

        // Calculate the inverse from the adjunct and the inverse determinate
        m_rows[ 0 ]  = adjRight * a;
        m_rows[ 1 ]  = adjUp * a;
        m_rows[ 2 ]  = adjAt * a;
        m_rows[ 3 ]  = adjPos * a;
    }

    //======================================================================================================================
    void Matrix4::SetRotationAA(const Vector3& axis, scalar_t angleRad) {
        // Method taken from :-
        // Graphics Gems (Glassner, Academic Press, 1990)

        scalar_t c = std::cos(angleRad);
        scalar_t s = std::sin(angleRad);
        scalar_t t = 1.0f - c;

        // Cache the axis components
        scalar_t x = axis.X();
        scalar_t y = axis.Y();
        scalar_t z = axis.Z();

        // Pre-calculate various terms to help with readability
        scalar_t sx = s * x;
        scalar_t sy = s * y;
        scalar_t sz = s * z;

        scalar_t tx2 = t * ( x * x );
        scalar_t ty2 = t * ( y * y );
        scalar_t tz2 = t * ( z * z );

        scalar_t txy = t * x * y;
        scalar_t txz = t * x * z;
        scalar_t tyz = t * y * z;

        // Set the rows of the matrix
        m_rows[ 0 ].Set(tx2 + c, txy + sz, txz - sy, 0);
        m_rows[ 1 ].Set(txy - sz, ty2 + c, tyz + sx, 0);
        m_rows[ 2 ].Set(txz + sy, tyz - sx, tz2 + c, 0);
    }

    //======================================================================================================================
    void Matrix4::SetRotationQ(const Quaternion& rhs) {
        scalar_t xx = scalar_t( 2.0f ) * rhs.X() * rhs.X();
        scalar_t yy = scalar_t( 2.0f ) * rhs.Y() * rhs.Y();
        scalar_t zz = scalar_t( 2.0f ) * rhs.Z() * rhs.Z();
        
        scalar_t xw = scalar_t( 2.0f ) * rhs.X() * rhs.W();
        scalar_t yw = scalar_t( 2.0f ) * rhs.Y() * rhs.W();
        scalar_t zw = scalar_t( 2.0f ) * rhs.Z() * rhs.W();
        scalar_t xy = scalar_t( 2.0f ) * rhs.X() * rhs.Y();
        scalar_t xz = scalar_t( 2.0f ) * rhs.X() * rhs.Z();
        scalar_t yz = scalar_t( 2.0f ) * rhs.Y() * rhs.Z();
        
        m_rows[ 0 ].Set   (scalar_t(1) - (yy + zz), xy + zw, xz - yw, 0);
        m_rows[ 1 ].Set      (xy - zw, scalar_t(1) - (xx + zz), yz + xw, 0);
        m_rows[ 2 ].Set      (xz + yw, yz - xw, scalar_t(1) - (xx + yy), 0);
    }

    //======================================================================================================================
    void Matrix4::SetProjectionOrthoLH(scalar_t lWidth, scalar_t lHeight, scalar_t lNear, scalar_t lFar) {
        m_rows[ 0 ].Set     (2.0f/lWidth, 0, 0, 0);
        m_rows[ 2 ].Set        (0, 2.0f/lHeight, 0, 0);
        m_rows[ 1 ].Set        (0, 0, 2.0f/(lFar-lNear), 0);
            
        m_rows[ 3 ].Set( -( lWidth / lWidth),
                        -( lHeight / lHeight ),
                        -( (lFar+lNear ) / ( lFar-lNear ) ), 1);
    }
        
    //======================================================================================================================
    void Matrix4::SetProjectionPerspLH(scalar_t fov, scalar_t aspect, scalar_t nearClip, scalar_t farClip) {
        /*
            Coordinate system defined by projection matrix is:-
            
            +y
            |  +z
            | /
            |/_ _ _ +x
            
            */
            
        scalar_t yScale = 1.0f / std::tan(fov * 0.5f);
        scalar_t xScale = yScale / aspect;
            
        m_rows[ 0 ].Set   (xScale,       0,          0,                                          0);
        m_rows[ 1 ].Set   (0,            yScale,     0,                                          0);
        m_rows[ 2 ].Set   (0,            0,          farClip / (farClip-nearClip),               1);
        m_rows[ 3 ].Set   (0,            0,          -nearClip*farClip / (farClip-nearClip),     0);
            
        //scalar_t lTmp = 1.0f / (lFarClip - lNearClip);
        // scale z to (-w, +w) range (different than Wii's -w...0 range
        //m_rows[2].set(0,         0,      (lFarClip + lNearClip) * lTmp, -1);
        //m_rows[3].set(0,         0,      -(lFarClip*lNearClip) * lTmp, 0);
    }

    //======================================================================================================================
    void Matrix4::SetProjectionPerspLHInf(scalar_t fov, scalar_t aspect, scalar_t nearClip, scalar_t farClip) {
        scalar_t yScale = 1.0f / std::tan(fov * 0.5f);
        scalar_t xScale = yScale / aspect;

        m_rows[ 0 ].Set(xScale,    0,        0,            0);
        m_rows[ 1 ].Set(0,        yScale, 0,            0);
        m_rows[ 2 ].Set(0,        0,        0,            1);
        m_rows[ 3 ].Set(0,        0,        nearClip,    0);
    }
        
    //======================================================================================================================
    void Matrix4::SetLookAt(const Vector3& eye, const Vector3& lookAt, const Vector3& up_) {
        Vector3 lookDir, right_, actualUp;
            
        lookDir = lookAt - eye;
            
        right_.Cross( up_, lookDir);
        actualUp.Cross( lookDir, right_);
            
        right_.Normalise(right_ );
        lookDir.Normalise( lookDir );
        actualUp.Normalise( actualUp );
            
        m_rows[ 0 ] = Vector4(right_, 0);
        m_rows[ 1 ] = Vector4(actualUp, 0);
        m_rows[ 2 ] = Vector4(lookDir, 0);
        m_rows[ 3 ] = Vector4(eye, 1);
    }

    //======================================================================================================================
    void Matrix4::GetRotationQ( Quaternion & rot ) const {
        
        scalar_t r11 = m_rows[ 0 ].X();
        scalar_t r12 = m_rows[ 0 ].Y();
        scalar_t r13 = m_rows[ 0 ].Z();
        scalar_t r21 = m_rows[ 1 ].X();
        scalar_t r22 = m_rows[ 1 ].Y();
        scalar_t r23 = m_rows[ 1 ].Z();
        scalar_t r31 = m_rows[ 2 ].X();
        scalar_t r32 = m_rows[ 2 ].Y();
        scalar_t r33 = m_rows[ 2 ].Z();
        
        scalar_t q0 = ( r11 + r22 + r33 + 1.0f ) / 4.0f;
        scalar_t q1 = ( r11 - r22 - r33 + 1.0f ) / 4.0f;
        scalar_t q2 = ( -r11 + r22 - r33 + 1.0f ) / 4.0f;
        scalar_t q3 = ( -r11 - r22 + r33 + 1.0f ) / 4.0f;
        
        if (q0 < 0.0f) {
            q0 = 0.0f;
        }
        if (q1 < 0.0f) {
            q1 = 0.0f;
        }
        if (q2 < 0.0f) {
            q2 = 0.0f;
        }
        if (q3 < 0.0f) {
            q3 = 0.0f;
        }
        
        q0 = scalar::Sqrt(q0);
        q1 = scalar::Sqrt(q1);
        q2 = scalar::Sqrt(q2);
        q3 = scalar::Sqrt(q3);
        
        if ( q0 >= q1 && q0 >= q2 && q0 >= q3 ) {
            q0 *= +1.0f;
            q1 *= SIGN(r32 - r23);
            q2 *= SIGN(r13 - r31);
            q3 *= SIGN(r21 - r12);
        }
        else if ( q1 >= q0 && q1 >= q2 && q1 >= q3 ) {
            q0 *= SIGN(r32 - r23);
            q1 *= +1.0f;
            q2 *= SIGN(r21 + r12);
            q3 *= SIGN(r13 + r31);
        }
        else if ( q2 >= q0 && q2 >= q1 && q2 >= q3 ) {
            q0 *= SIGN(r13 - r31);
            q1 *= SIGN(r21 + r12);
            q2 *= +1.0f;
            q3 *= SIGN(r32 + r23);
        }
        else if ( q3 >= q0 && q3 >= q1 && q3 >= q2 ) {
            q0 *= SIGN(r21 - r12);
            q1 *= SIGN(r31 + r13);
            q2 *= SIGN(r32 + r23);
            q3 *= +1.0f;
        }
        else {
            rot = Quaternion::IDENTITY;
        }
        
        rot.Set( q1, q2, q3, q0 );
        rot.Normalise( rot );
    }
}

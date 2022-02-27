//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include <cmath>
#include "i3d/math/Math3d.h"

namespace i3d {

    #define calcCofactor(a, b, c, d) ((a * d) - (b * c))

    #define a0 rhs.m_rows[ 0 ].X()
    #define a1 rhs.m_rows[ 0 ].Y()
    #define a2 rhs.m_rows[ 0 ].Z()
    #define a3 rhs.m_rows[ 1 ].X()
    #define a4 rhs.m_rows[ 1 ].Y()
    #define a5 rhs.m_rows[ 1 ].Z()
    #define a6 rhs.m_rows[ 2 ].X()
    #define a7 rhs.m_rows[ 2 ].Y()
    #define a8 rhs.m_rows[ 2 ].Z()
    /*
        Above macros define the 3x3 matrix as
        a0 a1 a2
        a3 a4 a5
        a6 a7 a8
    */

    //======================================================================================================================
    void Matrix3::Inverse(const Matrix3 & rhs) {
        // Calculate the first three cofactors so we can also calculate the determinate of the matrix and early out if
        // need be.
        scalar_t c0 = calcCofactor( a4, a5, a7, a8 );
        scalar_t c1 = calcCofactor( a3, a5, a6, a8 );
        scalar_t c2 = calcCofactor( a3, a4, a6, a7 );
        
        scalar_t det = (a0 * c0) - (a1 * c1) + (a2 * c2);

        if ( scalar::Abs( det ) < 0.6e-5f ) {
            *this = Matrix3::IDENTITY;
            return;
        }

        scalar_t a = 1.0f / det;

        // If we get to here, we have a large enough determinate that we can use to calculate the inverse
        // so we need to calculate the rest of the coefficients
        scalar_t c3 = calcCofactor( a1, a2, a7, a8 );
        scalar_t c4 = calcCofactor( a0, a2, a6, a8 );
        scalar_t c5 = calcCofactor( a0, a1, a6, a7 );
        
        scalar_t c6 = calcCofactor( a1, a2, a4, a5 );
        scalar_t c7 = calcCofactor( a0, a2, a3, a5 );
        scalar_t c8 = calcCofactor( a0, a1, a3, a4 );

        // Create the adjunct matrix from the cofactors
        Vector3  adjRow0(  c0, -c3,  c6 );
        Vector3  adjRow1( -c1,  c4, -c7 );
        Vector3  adjRow2(  c2, -c5,  c8 );

        m_rows[ 0 ] = adjRow0 * a;
        m_rows[ 1 ] = adjRow1 * a;
        m_rows[ 2 ] = adjRow2 * a;
    }

    //======================================================================================================================
    void Matrix3::SetRotationQ( const Quaternion & rhs ) {
        
        scalar_t xx = scalar_t( 2.0f ) * rhs.X() * rhs.X();
        scalar_t yy = scalar_t( 2.0f ) * rhs.Y() * rhs.Y();
        scalar_t zz = scalar_t( 2.0f ) * rhs.Z() * rhs.Z();
        
        scalar_t xw = scalar_t( 2.0f ) * rhs.X() * rhs.W();
        scalar_t yw = scalar_t( 2.0f ) * rhs.Y() * rhs.W();
        scalar_t zw = scalar_t( 2.0f ) * rhs.Z() * rhs.W();
        scalar_t xy = scalar_t( 2.0f ) * rhs.X() * rhs.Y();
        scalar_t xz = scalar_t( 2.0f ) * rhs.X() * rhs.Z();
        scalar_t yz = scalar_t( 2.0f ) * rhs.Y() * rhs.Z();
        
        m_rows[ 0 ].Set   ( scalar_t(1) - (yy + zz), xy + zw, xz - yw );
        m_rows[ 1 ].Set   ( xy - zw, scalar_t(1) - (xx + zz), yz + xw );
        m_rows[ 2 ].Set   ( xz + yw, yz - xw, scalar_t(1) - (xx + yy) );
    }

    //======================================================================================================================
    void Matrix3::SetRotationAA( const Vector3 & axis, scalar_t angleRad ) {
        // Method taken from :-
        // Graphics Gems (Glassner, Academic Press, 1990)

        scalar_t c = scalar::Cos( angleRad );
        scalar_t s = scalar::Sin( angleRad );
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
        m_rows[ 0 ].Set( tx2 + c, txy + sz, txz - sy );
        m_rows[ 1 ].Set( txy - sz, ty2 + c, tyz + sx );
        m_rows[ 2 ].Set( txz + sy, tyz - sx, tz2 + c );
    }
}

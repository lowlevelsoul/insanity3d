/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Mat4.h"
#include "i3d/Quat.h"

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

#define a0 rhs->rows[ 0 ].x
#define a1 rhs->rows[ 0 ].y
#define a2 rhs->rows[ 0 ].z
#define a3 rhs->rows[ 0 ].w

#define a4 rhs->rows[ 1 ].x
#define a5 rhs->rows[ 1 ].y
#define a6 rhs->rows[ 1 ].z
#define a7 rhs->rows[ 1 ].w

#define a8  rhs->rows[ 2 ].x
#define a9  rhs->rows[ 2 ].y
#define a10 rhs->rows[ 2 ].z
#define a11 rhs->rows[ 2 ].w

#define a12 rhs->rows[ 3 ].x
#define a13 rhs->rows[ 3 ].y
#define a14 rhs->rows[ 3 ].z
#define a15 rhs->rows[ 3 ].w

/*
        a0  a1  a2  a3
        a4  a5  a6  a7
        a8  a9 a10 a11
        a12 a13 a14 a15
*/

/*======================================================================================================================================*/
void _Mat4_Inverse( Mat4_t * dst, const Mat4_t * rhs ) {
    // Calculate the first 4 cofactors so we can calculate the determinate and early out
    // if it is too small
    float c0, c1, c2, c3, c4;
    float c5, c6, c7, c8, c9;
    float c10, c11, c12, c13;
    float c14, c15, a, det;
    Vec4_t adj0, adj1, adj2, adj3;
    
    c0 = calcCofactor( a5, a6, a7, a9, a10, a11, a13, a14, a15 );
    c1 = calcCofactor( a4, a6, a7, a8, a10, a11, a12, a14, a15 );
    c2 = calcCofactor( a4, a5, a7, a8, a9, a11, a12, a13, a15 );
    c3 = calcCofactor( a4, a5, a6, a8, a9, a10, a12, a13, a14 );

    det = (a0 * c0) - (a1 * c1) + (a2 * c2) - (a3 * c3);
    if ( Scalar_Abs( det ) < 0.6e-5f ) {
        Mat4_SetIdentity( *dst );
        return;
    }

   a = 1.0f / det;

    // If we get to here, we have a large enough determinate that we can use to calculate the inverse
    // so we need to calculate the rest of the coefficients
   c4 = calcCofactor( a1, a2, a3, a9, a10, a11, a13, a14, a15 );
   c5 = calcCofactor( a0, a2, a3, a8, a10, a11, a12, a14, a15 );
   c6 = calcCofactor( a0, a1, a3, a8, a9, a11, a12, a13, a15 );
   c7 = calcCofactor( a0, a1, a2, a8, a9, a10, a12, a13, a14 );

   c8 = calcCofactor( a1, a2, a3, a5, a6, a7, a13, a14, a15 );
   c9 = calcCofactor( a0, a2, a3, a4, a6, a7, a12, a14, a15 );
   c10 = calcCofactor( a0, a1, a3, a4, a5, a7, a12, a13, a15 );
   c11 = calcCofactor( a0, a1, a2, a4, a5, a6, a12, a13, a14 );

   c12 = calcCofactor( a1, a2, a3, a5, a6, a7, a9, a10, a11 );
   c13 = calcCofactor( a0, a2, a3, a4, a6, a7, a8, a10, a11 );
   c14 = calcCofactor( a0, a1, a3, a4, a6, a7, a8, a9, a11 );
   c15 = calcCofactor( a0, a1, a2, a4, a5, a6, a8, a9, a10 );

    // Create the adjunct matrix as four vectors
    Vec4_Set(adj0,   c0,-c4, c8,-c12 );
    Vec4_Set(adj1,  -c1, c5,-c9, c13 );
    Vec4_Set(adj2,   c2,-c6, c10,-c14 );
    Vec4_Set(adj3,  -c3, c7,-c11, c15 );

    // Calculate the inverse from the adjunct and the inverse determinate
    Vec4_MulScalar( dst->rows[ 0 ], adj0, a );
    Vec4_MulScalar( dst->rows[ 1 ], adj1, a );
    Vec4_MulScalar( dst->rows[ 2 ], adj2, a );
    Vec4_MulScalar( dst->rows[ 3 ], adj3, a );
}

/*======================================================================================================================================*/
void _Mat4_Concat( Mat4_t * dst, const Mat4_t * lhs, const Mat4_t * rhs ) {
    _Mat4_Transform( &dst->rows[0], rhs, &lhs->rows[0] );
    _Mat4_Transform( &dst->rows[1], rhs, &lhs->rows[1] );
    _Mat4_Transform( &dst->rows[2], rhs, &lhs->rows[2] );
    _Mat4_Transform( &dst->rows[3], rhs, &lhs->rows[3] );
}

/*======================================================================================================================================*/
void _Mat4_Transform( Vec4_t * dst, const Mat4_t * xform, const Vec4_t * vec ) {
    Vec4_t tmp1, tmp2, tmp3;
    
    Vec4_MulScalar( *dst, xform->rows[0], vec->x );
    Vec4_MulScalar( tmp1, xform->rows[1], vec->y );
    Vec4_MulScalar( tmp2, xform->rows[2], vec->z );
    Vec4_MulScalar( tmp3, xform->rows[3], vec->w );
    
    Vec4_Add( *dst, *dst, tmp1 );
    Vec4_Add( *dst, *dst, tmp2 );
    Vec4_Add( *dst, *dst, tmp3 );
}

/*======================================================================================================================================*/
void _Mat4_SetRotationAA( Mat4_t * dst, const Vec3_t * axis, float angleRad ) {
    /* Method taken from :-
       Graphics Gems (Glassner, Academic Press, 1990)
    */

    float c = Scalar_Cos( angleRad );
    float s = Scalar_Sin( angleRad );
    float t = 1.0f - c;

    float sx = s * axis->x;
    float sy = s * axis->y;
    float sz = s * axis->z;

    float tx2 = t * ( axis->x * axis->x );
    float ty2 = t * ( axis->y * axis->y );
    float tz2 = t * ( axis->z * axis->z );

    float txy = t * axis->x * axis->y;
    float txz = t * axis->x * axis->z;
    float tyz = t * axis->y * axis->z;

    /* Set the rows of the matrix */
    Vec4_Set( dst->rows[ 0 ], tx2 + c, txy + sz, txz - sy, 0 );
    Vec4_Set( dst->rows[ 1 ], txy - sz, ty2 + c, tyz + sx, 0 );
    Vec4_Set( dst->rows[ 2 ], txz + sy, tyz - sx, tz2 + c, 0 );
}

/*======================================================================================================================================*/
void _Mat4_SetRotationQ( Mat4_t * dst, const Quat_t * quat ) {
    float xx =  2.0f * quat->x * quat->x;
    float yy =  2.0f * quat->y * quat->y;
    float zz =  2.0f * quat->z * quat->z;
    
    float xw =  2.0f * quat->x * quat->w;
    float yw =  2.0f * quat->y * quat->w;
    float zw =  2.0f * quat->z * quat->w;
    float xy =  2.0f * quat->x * quat->y;
    float xz =  2.0f * quat->x * quat->z;
    float yz =  2.0f * quat->y * quat->z;
    
    Vec4_Set( dst->rows[ 0 ], 1.0f - (yy + zz), xy + zw, xz - yw, 0 );
    Vec4_Set( dst->rows[ 1 ], xy - zw, 1.0f - (xx + zz), yz + xw, 0 );
    Vec4_Set( dst->rows[ 2 ], xz + yw, yz - xw, 1.0f - (xx + yy), 0 );
}
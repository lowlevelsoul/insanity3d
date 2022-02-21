/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Mat3.h"
#include "i3d/Quat.h"

#define calcCofactor(a, b, c, d) ((a * d) - (b * c))

#define a0 src->rows[ 0 ].x
#define a1 src->rows[ 0 ].y
#define a2 src->rows[ 0 ].z
#define a3 src->rows[ 1 ].x
#define a4 src->rows[ 1 ].y
#define a5 src->rows[ 1 ].z
#define a6 src->rows[ 2 ].x
#define a7 src->rows[ 2 ].y
#define a8 src->rows[ 2 ].z
/*
    Above macros define the 3x3 matrix as
    a0 a1 a2
    a3 a4 a5
    a6 a7 a8
*/

/*======================================================================================================================================*/
void _Mat3_Concat( Mat3_t * dst, const Mat3_t * lhs, const Mat3_t * rhs ) {
    _Mat3_Transform( &dst->rows[0], rhs, &lhs->rows[0] );
    _Mat3_Transform( &dst->rows[1], rhs, &lhs->rows[1] );
    _Mat3_Transform( &dst->rows[2], rhs, &lhs->rows[2] );
}

/*======================================================================================================================================*/
void _Mat3_Transform( Vec3_t * dst, const Mat3_t * xform, const Vec3_t * vec ) {
    Vec3_t tmp0, tmp1;
    Vec3_MulScalar( *dst, xform->rows[0], vec->x);
    Vec3_MulScalar(tmp0, xform->rows[1], vec->y);
    Vec3_MulScalar(tmp1, xform->rows[2], vec->z);
    Vec3_Add( *dst, *dst, tmp0 );
    Vec3_Add( *dst, *dst, tmp1 );
}

/*======================================================================================================================================*/
void _Mat3_Inverse( Mat3_t * dst, const Mat3_t * src ) {
    /* Calculate the first three cofactors so we can also calculate the determinate of the matrix and early out if
      need be. */
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
    Vec3_t adjRow0, adjRow1, adjRow2;
    float a;
    
    c0 = calcCofactor( a4, a5, a7, a8 );
    c1 = calcCofactor( a3, a5, a6, a8 );
    c2 = calcCofactor( a3, a4, a6, a7 );
    
    float det = (a0 * c0) - (a1 * c1) + (a2 * c2);

    if ( Scalar_Abs( det ) < 0.6e-5f ) {
        Mat3_SetIdentity( *dst );
        return;
    }

    a = 1.0f / det;

    /* If we get to here, we have a large enough determinate that we can use to calculate the inverse
       so we need to calculate the rest of the coefficients */
    c3 = calcCofactor( a1, a2, a7, a8 );
    c4 = calcCofactor( a0, a2, a6, a8 );
    c5 = calcCofactor( a0, a1, a6, a7 );
    
    c6 = calcCofactor( a1, a2, a4, a5 );
    c7 = calcCofactor( a0, a2, a3, a5 );
    c8 = calcCofactor( a0, a1, a3, a4 );

    /* Create the adjunct matrix from the cofactors */
    Vec3_Set( adjRow0,  c0, -c3,  c6 );
    Vec3_Set( adjRow1, -c1,  c4, -c7 );
    Vec3_Set( adjRow2,  c2, -c5,  c8 );

    Vec3_MulScalar( dst->rows[ 0 ], adjRow0, a );
    Vec3_MulScalar( dst->rows[ 1 ], adjRow1, a );
    Vec3_MulScalar( dst->rows[ 2 ], adjRow2, a );
}

/*======================================================================================================================================*/
void _Mat3_SetRotationAA( Mat3_t * dst, const Vec3_t * axis, float angleRad ) {
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
    Vec3_Set( dst->rows[ 0 ], tx2 + c, txy + sz, txz - sy );
    Vec3_Set( dst->rows[ 1 ], txy - sz, ty2 + c, tyz + sx );
    Vec3_Set( dst->rows[ 2 ], txz + sy, tyz - sx, tz2 + c );
}

/*======================================================================================================================================*/
void _Mat3_SetRotationQ( Mat3_t * dst, const Quat_t * quat ) {
    
    float xx =  2.0f * quat->x * quat->x;
    float yy =  2.0f * quat->y * quat->y;
    float zz =  2.0f * quat->z * quat->z;
    
    float xw =  2.0f * quat->x * quat->w;
    float yw =  2.0f * quat->y * quat->w;
    float zw =  2.0f * quat->z * quat->w;
    float xy =  2.0f * quat->x * quat->y;
    float xz =  2.0f * quat->x * quat->z;
    float yz =  2.0f * quat->y * quat->z;
    
    Vec3_Set( dst->rows[ 0 ], 1.0f - (yy + zz), xy + zw, xz - yw );
    Vec3_Set( dst->rows[ 1 ], xy - zw, 1.0f - (xx + zz), yz + xw );
    Vec3_Set( dst->rows[ 2 ], xz + yw, yz - xw, 1.0f - (xx + yy) );
}
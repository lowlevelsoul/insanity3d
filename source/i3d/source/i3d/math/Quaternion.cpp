//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include <cmath>
#include "i3d/math/Math3d.h"

namespace i3d {

    //======================================================================================================================
    Quaternion& Quaternion::Slerp( const Quaternion& from, const Quaternion& to, scalar_t t ) {
        scalar_t cosOmega;
        scalar_t q1x;
        scalar_t q1y;
        scalar_t q1z;
        scalar_t q1w;
        scalar_t k0, k1;
        scalar_t sinOmega, omega, oneOverSinOmega;
        
        // Limit interval to 0 and 1
        if ( t <= 0 ) {
            *this = from;
            return *this;
        }
        
        if ( t >= 1 ) {
            *this = to;
            return *this;
        }
        
        // Compute "cosine of angle between quaternions" using dot product
        cosOmega = from.Dot( to );
        
        // If negative dot, use -q1.  Two quaternions q and -q represent
        // the same rotation, but may produce different slerp.  We chose
        // q or -q to rotate using the acute angle.
        q1x = to.m_x;
        q1y = to.m_y;
        q1z = to.m_z;
        q1w = to.m_w;
        
        if( cosOmega < 0 ) {
            q1x = -q1x;
            q1y = -q1y;
            q1z = -q1z;
            q1w = -q1w;
            cosOmega = -cosOmega;
        }
        
        // Compute interpolation fraction, checking for quaternions almost exactly the same
        if ( cosOmega > 0.9999f ) {
            // Very close - just use linear interpolation, which will protect against a divide by zero
            k0 = 1.0f - t;
            k1 = t;
        } else {
            // Compute the sin of the angle using the trig identity sin^2(omega) + cos^2(omega) = 1
            sinOmega = scalar::Sqrt( scalar_t(1) - ( cosOmega * cosOmega ) );
            
            // Compute the angle from its sin and cosine
            omega = scalar::Atan2( sinOmega, cosOmega );
            
            // Compute inverse of denominator, so we only have to divide once
            oneOverSinOmega = scalar_t(1) / sinOmega;
            
            // Compute interpolation parameters
            k0 = scalar::Sin( ( scalar_t(1) - t ) * omega ) * oneOverSinOmega;
            k1 = scalar::Sin( t * omega ) * oneOverSinOmega;
        }
        
        // Interpolate and return new quaternion
        m_x = ( k0 * from.m_x ) + ( k1 * q1x );
        m_y = ( k0 * from.m_y ) + ( k1 * q1y );
        m_z = ( k0 * from.m_z ) + ( k1 * q1z );
        m_w = ( k0 * from.m_w ) + ( k1 * q1w );
        
        return *this;
    }
    
#define m00 rhs.m_rows[0].X()
#define m01 rhs.m_rows[0].Y()
#define m02 rhs.m_rows[0].Z()
    
#define m10 rhs.m_rows[1].X()
#define m11 rhs.m_rows[1].Y()
#define m12 rhs.m_rows[1].Z()
    
#define m20 rhs.m_rows[2].X()
#define m21 rhs.m_rows[2].Y()
#define m22 rhs.m_rows[2].Z()
    
    //======================================================================================================================
    void Quaternion::SetFromMatrix( const Matrix3 & rhs ) {
        float s, w, x, y, z;
        float tr = m00 + m11 + m22;
        
        if ( tr > 0 )  {
            
            s = scalar::Sqrt( 1.0f + tr) * 2.0f;
            w = 0.25f * s;
            x = (m12 - m21) / s;
            y = (m20 - m02) / s;
            z = (m01 - m10) / s;
            
        }
        else if ( ( m00 > m11 ) && ( m00 > m22 ) ) {
            
            s = scalar::Sqrt(1.0 + m00 - m11 - m22) * 2; // S=4*qx
            w = (m12 - m21) / s;
            x = 0.25 * s;
            y = (m10 + m01) / s;
            z = (m20 + m02) / s;
            
        } else if ( m11 > m22 ) {
            
            s = scalar::Sqrt(1.0 + m11 - m00 - m22) * 2; // S=4*qy
            w = (m20 - m02) / s;
            x = (m10 + m01) / s;
            y = 0.25 * s;
            z = (m21 + m12) / s;
            
        } else {
            
            s = scalar::Sqrt(1.0 + m22 - m00 - m11) * 2; // S=4*qz
            w = (m01 - m10) / s;
            x = (m20 + m02) / s;
            y = (m21 + m12) / s;
            z = 0.25 * s;
        }
        
        Set( x, y, z, w );
    }
    
    //======================================================================================================================
    void Quaternion::SetFromMatrix( const Matrix4 & rhs ) {
        float s, w, x, y, z;
        float tr = m00 + m11 + m22;
        
        if ( tr > 0 )  {
            
            s = scalar::Sqrt( 1.0f + tr) * 2.0f;
            w = 0.25f * s;
            x = (m12 - m21) / s;
            y = (m20 - m02) / s;
            z = (m01 - m10) / s;
            
        }
        else if ( ( m00 > m11 ) && ( m00 > m22 ) ) {
            
            s = scalar::Sqrt(1.0 + m00 - m11 - m22) * 2; // S=4*qx
            w = (m12 - m21) / s;
            x = 0.25 * s;
            y = (m10 + m01) / s;
            z = (m20 + m02) / s;
            
        } else if ( m11 > m22 ) {
            
            s = scalar::Sqrt(1.0 + m11 - m00 - m22) * 2; // S=4*qy
            w = (m20 - m02) / s;
            x = (m10 + m01) / s;
            y = 0.25 * s;
            z = (m21 + m12) / s;
            
        } else {
            
            s = scalar::Sqrt(1.0 + m22 - m00 - m11) * 2; // S=4*qz
            w = (m01 - m10) / s;
            x = (m20 + m02) / s;
            y = (m21 + m12) / s;
            z = 0.25 * s;
        }
        
        Set( x, y, z, w );
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLANE_H__
#define __PLANE_H__

namespace i3d {
    
    class Plane {
    public:
        enum TEST_RESULT {
            TEST_RESULT_HIT=0,
            TEST_RESULT_MISS,
            TEST_RESULT_PARALLEL,
        };
        
        Plane() {
            // Empty - does nothing
        }
        
        Plane( const Vector3 & n, float d ) {
            Set( n, d );
        }
        
        Plane( const Plane & rhs ) {
            m_value = rhs.m_value;
        }
        
        void Set( const Vector3 & n, float d ) {
            m_value.Set(n, d );
        }
        
        void Set( const Vector3 & v0, const Vector3 & v1, const Vector3 & v2 ) {
            Vector3 e0 = v0 - v1;
            Vector3 e1 = v1 - v2;
            
            Vector3 n;
            n.Cross( e0, e1 );
            n.Normalise();
            
            float d = n.Dot( v0 );
            Set( n, d );
        }
        
        Vector3 GetNormal() const { return m_value; }
        
        float GetDistance() const { return m_value.W(); }
        
        void SetNormal( const Vector3 & n ) {
            m_value.Set(n, m_value.W() );
        }
        
        void SetDistance( float d ) {
            m_value.W() = d;
        }
        
        Vector3 ClipPoint(const Vector3 & p) const;
        
        TEST_RESULT TestSegment(Vector3& q, float& t, const Vector3& p0, const Vector3& p1) const;
        
    protected:
        Vector4         m_value;
    };
    
    //======================================================================================================================
    inline Vector3 Plane::ClipPoint( const Vector3 & p ) const {
        i3d::Vector3 n = m_value;
        
        float d = n.Dot( p ) - m_value.W();
        if (d > 0) {
            Vector3 cp = p - n * d;
            return cp;
        }
        
        return p;
    }
    
    //======================================================================================================================
    inline Plane::TEST_RESULT Plane::TestSegment( Vector3 & q, float & t, const Vector3 & p0, const Vector3 & p1 ) const {
        Vector3 ab = p1 - p0;
        float nDotAb = GetNormal().Dot(ab);
        
        if ( nDotAb == 0 ) {
            return TEST_RESULT_PARALLEL;
        }
        
        t = ( GetDistance() - GetNormal().Dot( p0 ) ) / nDotAb;
        
        if ( ( t >= 0 ) && ( t <= 1.0f ) ) {
            q = p0 + ( ab * t );
            return TEST_RESULT_HIT;
        }
        
        return TEST_RESULT_MISS;
    }
}


#endif 

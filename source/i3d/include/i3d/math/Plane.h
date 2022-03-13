//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLANE_H__
#define __PLANE_H__

namespace i3d {
    
    class Plane {
    public:
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
        
    protected:
        Vector4         m_value;
    };
}


#endif 

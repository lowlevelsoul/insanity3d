//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SPHERE_H__
#define __I3D_SPHERE_H__

namespace i3d {
    
    class Sphere {
    public:
        Sphere() {
            // Empty - does nothing
        }
        
        Sphere( const Vector3 & c, float radius ) {
            Set( c, radius );
        }
        
        Sphere( const Sphere & rhs ) {
            m_value = rhs.m_value;
        }
        
        void Set( const Vector3 & c, float radius ) {
            m_value.Set( c, radius );
        }
        
        Vector3 GetCenter() const { return m_value; }
        
        float GetRadius() const { return m_value.W(); }
        
        void SetCenter( const Vector3 & c ) {
            m_value.Set( c, m_value.W() );
        }
        
        void SetRadius( float radius ) {
            m_value.W() = radius;
        }
        
    protected:
        Vector4         m_value;
    };
}


#endif

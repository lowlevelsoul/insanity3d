//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_FRUSTUM_H__
#define __I3D_FRUSTUM_H__

#include "i3d/math/Plane.h"

namespace i3d {
    
    class Frustum {
    public:
        enum PLANE {
            PLANE_NEAR = 0,
            PLANE_FAR,
            PLANE_RIGHT,
            PLANE_LEFT,
            PLANE_TOP,
            PLANE_BOTTOM,
            PLANE_COUNT
        };
        
        Frustum();
        
        ~Frustum();
        
        void Set( float fovDeg, float aspect, float nearClip, float farClip );
        
        void Set( float fovDeg, float aspect, float nearClip, float farClip, const Matrix4 & xform, bool useXform );
        
        Vector3 ClipPoint( const Vector3 & p );
        
    protected:
        Plane       m_planes[ PLANE_COUNT ];
    };
}


#endif

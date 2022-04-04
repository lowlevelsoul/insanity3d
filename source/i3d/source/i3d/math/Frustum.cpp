//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/math/Frustum.h"


namespace i3d {
    
    static const Vector3 PLANE_VERTEX_POS[] {
        Vector3( -1,  1,  1 ),
        Vector3(  1,  1,  1 ),
        Vector3(  1, -1,  1 ),
        Vector3( -1, -1,  1 )
    };
    
    static const uint32_t PLANE_FACES[ Frustum::PLANE_COUNT ][ 4 ] = {
        0, 1, 2, 3,
        7, 6, 5, 4,
        1, 5, 6, 2,
        4, 0, 3, 7,
        4, 5, 1, 0,
        3, 2, 6, 7
    };
        
    //======================================================================================================================
    Frustum::Frustum() {
        
    }
    
    //======================================================================================================================
    Frustum::~Frustum() {
        
    }
    
    //======================================================================================================================
    void Frustum::Set( float fovDeg, float aspect, float nearClip, float farClip ) {
        Set( fovDeg, aspect, nearClip, farClip, i3d::Matrix4::IDENTITY, false );
    }
    
    //======================================================================================================================
    void Frustum::Set( float fovDeg, float aspect, float nearClip, float farClip, const Matrix4 & xform, bool useXform ) {
        float scaleY = i3d::scalar::Tan( i3d::scalar::DegToRad( fovDeg ) / 2.0f );
        float scaleX = scaleY * aspect;
        
        // Construct the frustum vertices in local space
        Vector3 nearVert( scaleX * nearClip, scaleY * nearClip, nearClip );
        Vector3 farVert( scaleX * farClip, scaleY * farClip, farClip );
        
        Vector3 localVerts[8] = {
            nearVert * PLANE_VERTEX_POS[0],
            nearVert * PLANE_VERTEX_POS[1],
            nearVert * PLANE_VERTEX_POS[2],
            nearVert * PLANE_VERTEX_POS[3],
            farVert * PLANE_VERTEX_POS[0],
            farVert * PLANE_VERTEX_POS[1],
            farVert * PLANE_VERTEX_POS[2],
            farVert * PLANE_VERTEX_POS[3]
        };
        
        if ( useXform == true ) {
        
            // Transform the frustum verts into world space
            Vector3 xformVerts[8];
            for ( uint32_t i = 0; i < 8; ++i ) {
                xform.Transform( xformVerts[i], localVerts[i] );
            }
            
            /* Calculate the planes */
            for ( uint32_t i = 0; i < PLANE_COUNT; ++i ) {
                const Vector3 & v0 = xformVerts[ PLANE_FACES[ i ][ 0 ] ];
                const Vector3 & v1 = xformVerts[ PLANE_FACES[ i ][ 1 ] ];
                const Vector3 & v2 = xformVerts[ PLANE_FACES[ i ][ 2 ] ];
                m_planes[ i ].Set( v0, v1, v2 );
            }
        }
        else {
            /* Calculate the planes */
            for ( uint32_t i = 0; i < PLANE_COUNT; ++i ) {
                const Vector3 & v0 = localVerts[ PLANE_FACES[ i ][ 0 ] ];
                const Vector3 & v1 = localVerts[ PLANE_FACES[ i ][ 1 ] ];
                const Vector3 & v2 = localVerts[ PLANE_FACES[ i ][ 2 ] ];
                m_planes[ i ].Set( v0, v1, v2 );
            }
        }
    }
    
    
    //======================================================================================================================
    Vector3 Frustum::ClipPoint( const Vector3 & point ) {
        
        Vector3 currP = point;
        
        for ( uint32_t ip = 0; ip < PLANE_COUNT; ++ip ) {
            const Plane & p = m_planes[ip ];
            Vector3 n = p.GetNormal();
            
            float d = n.Dot( currP ) - p.GetDistance();
            if ( d > 0 ) {
                currP -= ( n * d );
            }
        }
        
        return currP;
    }
    
    //======================================================================================================================
    void Frustum::ClipPoint( Vector3& clippedPoint, const Vector3 & p) {
        
        clippedPoint = p;
        
        for(uint32_t i=0; i < PLANE_COUNT; ++i) {
            clippedPoint = m_planes[ i ].ClipPoint(clippedPoint);
        }
    }

    //======================================================================================================================
    bool Frustum::ClipSegment( Vector3 & clippedPoint, const Vector3 & p0, const Vector3 & p1 ) {
        
        float closestT = 1;
        bool haveHit = false;
        
        for(uint32_t i=0; i < PLANE_COUNT; ++i) {
            Vector3 q;
            float t;
            Plane::TEST_RESULT hit = m_planes[i].TestSegment(q, t, p0, p1);
            if (hit == Plane::TEST_RESULT_HIT &&  t < closestT) {
                closestT = t;
                haveHit = true;
            }
        }
        
        if (haveHit == true) {
            clippedPoint = ( p0 * ( 1.0f - closestT ) ) + ( p1 * closestT );
        }
        
        return haveHit;
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYFIELD_H__
#define __PLAYFIELD_H__

class Camera;

class Playfield : public i3d::RttiObject {
public:
    RTTI_CLASS_DECLARE( Playfield, i3d::RttiObject )
    
    Playfield();
    
    virtual ~Playfield();
    
    void Update( Camera* camera);
    
    void CalcZLimits( float & znear, float & zfar, float height, float xpos );
    
    void CalcXLimits( float & left, float & right, float height, float zpos );
    
    void CalcGroundPoint();
    
    void CalcNormalisedToPlayfieldPoint( i3d::Vector2 & p, const i3d::Vector2 & np );
    
public:
    float               m_playHeight;               ///< Height on the y-axis at which the actiun takes place
    i3d::Matrix4        m_cameraTransform;          ///< Camera matrix used to calculate the transform
    float               m_cameraNear;
    float               m_cameraFar;
    i3d::Frustum        m_frustum;                  ///< View frustum
    i3d::Vector3        m_cameraGroundPoint;        ///< Point at which the camera look-vector hits the ground plane
    bool                m_cameraGroundPointValid;   ///< Flag to tell us the the ground hit point is valid
    
    i3d::Vector2        m_boundsMin;
    i3d::Vector2        m_boundsMax;
    
    i3d::Vector2        m_limitsXNear;
    i3d::Vector2        m_limitsXFar;

};

extern Playfield * playfield;

#endif

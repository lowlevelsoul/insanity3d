//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

struct Transform {
    i3d::Vector3        m_location;
    i3d::Quaternion     m_rotation;
    i3d::Matrix4        m_transform;
};

class TransformPrototype : public i3d::ComponentPrototype {
public:
    RTTI_CLASS_DECLARE_NAMED( TransformPrototype, i3d::ComponentPrototype )
    
    TransformPrototype();
    
    virtual ~TransformPrototype();
    
    virtual void CreateComponent( i3d::Entity ent );
    
    virtual void Construct( i3d::Entity & ent );
    
    virtual void DestroyComponent( i3d::Entity ent );
    
public:
    i3d::Vector3        m_location;
    i3d::Quaternion     m_rotation;
};

#endif

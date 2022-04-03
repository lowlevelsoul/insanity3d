//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIP_H__
#define __SHIP_H__

#include "entity/Entity.h"

class Ship;

class ShipComponent : public i3d::RttiObject {
    friend class Ship; 
public:
    RTTI_CLASS_DECLARE( ShipComponent, i3d::RttiObject );
    
    typedef i3d::stl::Vector<ShipComponent::ref_ptr_t> vector_t;
    
    ShipComponent() {
        m_owner = nullptr;
    }
    
    virtual ~ShipComponent() {
        // Empty - does nothing
    }
    
    virtual void Think( float timeStep ) {
        // Empty - does nothing
    }
    
    Ship * GetOwner() { return m_owner; }
    
    const Ship * GetOwner() const { return m_owner; }
    
protected:
    Ship *          m_owner;
};

class Ship : public Entity {
public:
    RTTI_CLASS_DECLARE( Ship, Entity );
    
    Ship();
    
    ~Ship();
    
    virtual void Think( float timeStep ) override;
    
    virtual void Draw( float timeStep ) override;
    
    virtual bool ReadComplete( const char * path ) override;
    
public:
    ShipComponent::vector_t::type   m_components;
    i3d::Model *                    m_model;
    i3d::MaterialResource *         m_materialMat;
    
    i3d::Quaternion                 m_modelOffsRot;
    i3d::Vector3                    m_modelOffsPos;
    i3d::Matrix4                    m_transform;
};

#endif

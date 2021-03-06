//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIP_H__
#define __SHIP_H__

#include "entity/Entity.h"

class Ship;
class ShipDef;
class ShipComponentDef;

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
    
    virtual void Construct( ShipComponentDef * compDef ) {
        // Empty - does nothing
    }
    
    void SetOwner( Ship * owner ) { m_owner = owner; }
    
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
    
    virtual void Construct( EntityDef * def ) override;
    
    bool FindComponent( uint32_t & index, i3d::RttiType * type );
    
    bool FindNextComponent( uint32_t & index, i3d::RttiType * type );
    
    ShipComponent * FindComponent( i3d::RttiType * type );
    
    template<typename __type__>
    __type__ * FindComponent() {
        ShipComponent * comp = FindComponent( __type__::GetClassType() );
        if ( comp == nullptr ) {
            return nullptr;
        }
        
        return ( __type__ * ) comp;
    }

public:
    ShipComponent::vector_t::type   m_components;
    i3d::Model *                    m_model;
    i3d::Material *                 m_material;
    
    i3d::Quaternion                 m_modelOffsRot;
    i3d::Vector3                    m_modelOffsPos;
    i3d::Matrix4                    m_transform;
    
    float                           m_roll;
};

#endif

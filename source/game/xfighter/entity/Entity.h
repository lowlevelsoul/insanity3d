//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "i3d/core/IntrusiveList.h"

class Entity : public i3d::RttiObject {
    friend class EntityManager;
public:
    RTTI_CLASS_DECLARE( Entity, i3d::RttiObject )
    
    typedef uint64_t id_t;
    typedef i3d::IntrusiveList<Entity> list_node_t;
    typedef i3d::stl::Map<Entity::id_t, Entity *> map_t;
    static const id_t MAX_ENTITIES = 2048;
    static const id_t ID_INVALID = -1;
    
    struct Flags {
        uint64_t        m_active : 1;
        uint64_t        m_remove : 1;
        
        Flags() {
            m_active = 0;
            m_remove = 0;
        }
    };
    
    Entity();
    
    virtual ~Entity();
    
    virtual void OnAdd();
    
    virtual void OnDelete();
    
    virtual void Think( float timeStep );
    
    virtual void Draw( float timeStep );
    
    const i3d::Vector3 & GetLocation() const { return m_location; }
    void SetLocation( const i3d::Vector3 & value ) { m_location = value; }
    
    const i3d::Quaternion & GetRotation() const { return m_rotation; }
    void SetRotation( const i3d::Quaternion & value ) { m_rotation = value; }
    
protected:
    id_t                m_id;           ///< Id / index of the entity
    list_node_t         m_deleteNode;   ///< Node use for when the item needs to be deleted
    Flags               m_flags;        ///< Entity state flags
                                        ///
    bool                m_doThink;      ///
    bool                m_doDraw;       ///
    i3d::Vector3        m_location;     ///< Location of the entty in the playfield
    i3d::Quaternion     m_rotation;     ///< Orientation of the entirty in the playfield
};

#endif

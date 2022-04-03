//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"

class EntityManager {
public:
    EntityManager();
    
    ~EntityManager();
    
    void AddEntity( Entity * ent );
    
    void RemoveEntity( Entity * ent );
    
    void Think( float timeStep );
    
    void Draw( float timeStep, int viewWidth, int viewHeight );
    
protected:
    
    void ProcessDeleteList();
    
protected:
    Entity::id_t                        m_nextId;
    Entity::map_t::type                 m_entities;
    Entity::list_node_t                 m_deleteList;
    size_t                              m_activeCount;
};

extern EntityManager * entityMgr;

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "EntityManager.h"

EntityManager * entityMgr = nullptr;

//======================================================================================================================
void EntityManagerCreate() {
    if ( entityMgr == nullptr ) {
        entityMgr = new EntityManager;
    }
}

//======================================================================================================================
void EntotyManagerDestroy() {
    SAFE_DELETE( entityMgr );
}

//======================================================================================================================
EntityManager::EntityManager() {
    m_activeCount = 0;
    m_nextId = 0;
    
}

//======================================================================================================================
EntityManager::~EntityManager() {
    
}

//======================================================================================================================
void EntityManager::Think( float timeStep ) {
    ProcessDeleteList();
    
    for ( auto e : m_entities ) {
        Entity * ent = e.second;
        
        if (ent != nullptr && ent->m_doThink == true ) {
            ent->Think( timeStep );
        }
    }
}

//======================================================================================================================
void EntityManager::Draw( float timeStep, int viewWidth, int viewHeight ) {
    for ( auto e : m_entities ) {
        Entity * ent = e.second;
        
        if (ent != nullptr && ent->m_doDraw == true ) {
            ent->Draw( timeStep );
        }
    }
}

//======================================================================================================================
void EntityManager::AddEntity( Entity * ent ) {
    XE_ASSERT( ent->m_id == Entity::ID_INVALID );
    
    ent->m_id = m_nextId;
    ++m_nextId;
    
    m_entities[ent->m_id] = ent;
    ent->OnAdd();
}

//======================================================================================================================
void EntityManager::RemoveEntity( Entity * ent ) {
    XE_ASSERT( ent != nullptr );
    XE_ASSERT( ent->IsAlive() == true );        // Make sure we're not trying to remove an entity more than once
    
    m_deleteList.InsertBefore( &ent->m_deleteNode );
    ent->m_flags.m_remove = 1;
    ent->m_flags.m_active = 0;
}

//======================================================================================================================
void EntityManager::ProcessDeleteList() {
    if ( m_deleteList.Empty() == true ) {
        return;
    }
    
    while ( m_deleteList.Empty() == false ) {
        Entity * ent = *m_deleteList.Next();
        m_deleteList.Next()->Remove();
        
        m_entities.erase( ent->m_id );
        
        ent->OnDelete();
        delete ent;
    }
}

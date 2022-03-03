//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/ecs/ComponentArray.h"

namespace i3d {

    //======================================================================================================================
    ComponentArrayBase::ComponentArrayBase() {
        m_singleton = 0;
        m_capacity = 0;
        m_count = 0;
        m_refCount = 0;
        m_components = nullptr;
        m_componentStride = 0;
        m_prototypes = nullptr;
    }
    
    //======================================================================================================================
    ComponentArrayBase::~ComponentArrayBase() {
        
    }
    
    //======================================================================================================================
    void * ComponentArrayBase::AddEntity( Entity ent, ComponentPrototype * proto ) {
        
        uintptr_t index = 0;
        
        if ( m_singleton == 1 ) {
            // Singleton component. Bump up the reference count.
            ++m_refCount;
            
            if ( m_count == 0 ) {
                // First instance of the singleton component being used, so
                // set the prototype and bump the count.
                m_prototypes[0] = proto;
                ++m_count;
                
                // TODO - Tell component that we want to construct using its
                // prototype.
            }
            
            index = 0;
        }
        else {
            // Not a singleton component, so allocate normally
            XE_ASSERT( m_entityToIndexMap.find( ent) == m_entityToIndexMap.end() );
            
            index = (uintptr_t) m_count;
            ++m_count;
            
            m_entityToIndexMap[ ent ] = index;
            m_indexToEntityMap[ index ] = ent;
            m_prototypes[ index ] = proto;
        }
        
        return m_components[ index ];
    }
    
    //======================================================================================================================
    void ComponentArrayBase::RemoveEntity( Entity ent ) {
        if ( m_singleton == 1 ) {
            // Singleton - dec the reference count and check to see if this component
            // has been scrubbed.
            
        }
        else {
            XE_ASSERT( m_entityToIndexMap.find( ent ) != m_entityToIndexMap.end() );
            
            uintptr_t thisIndex = m_entityToIndexMap.find( ent )->second;
            
            if ( m_prototypes[ thisIndex ] != nullptr ) {
                // TODO - Destruct component using prototype
            }
            
            if ( m_count == 1 ) {
                m_indexToEntityMap.erase( ent );
                m_entityToIndexMap.erase( 0 );
            }
            else {
                uintptr_t endIndex = m_count - 1;
                Entity endEntity = m_indexToEntityMap[ endIndex ];
                
                // Overwrite the component in the place we're remvoing with the component that's at the end
                // of the component list
                CopyInternal( thisIndex, endIndex );
                m_prototypes[ thisIndex ] = m_prototypes[ endIndex ];
                m_entityToIndexMap[ endEntity ] = thisIndex;
                m_indexToEntityMap[ thisIndex ] = endEntity;
                
                // Remove the entries for the removed entity and the comonent that was
                // at the end of the component list
                m_indexToEntityMap.erase( endIndex );
                m_entityToIndexMap.erase( ent );
                
                --m_count;
            }
        }
    }
    
}

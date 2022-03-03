//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_COMPONENTARRAY_H__
#define __I3D_COMPONENTARRAY_H__

#include "i3d/core/Types.h"
#include "i3d/ecs/EcsTypes.h"

namespace i3d {
    
    class ComponentPrototype;
    
    class ComponentArrayBase {
    public:
        ComponentArrayBase();
        
        virtual ~ComponentArrayBase();
        
        void * AddEntity( Entity ent, ComponentPrototype * proto = nullptr  );
        
        template<typename __type__>
        __type__ * AddEntity( Entity ent, ComponentPrototype * proto = nullptr ) {
            XE_ASSERT( sizeof(__type__) == m_componentStride );
            return (__type__ *) AddEntity( ent, proto );
        }
        
        void RemoveEntity( Entity ent );
        
        void * GetComponent( Entity ent ) {
            if ( m_singleton == 1 ) {
                XE_ASSERT( m_count > 0 );
                return m_components[0];
            }
            
            XE_ASSERT( m_entityToIndexMap.find(ent) != m_entityToIndexMap.end() );
            return m_components[ m_entityToIndexMap.find(ent)->second ];
        }
        
        template<typename __type__>
        __type__ * GetComponent( Entity ent ) {
            return ( __type__ * ) GetComponent( ent );
        }
        
    protected:
        
        virtual void CopyInternal( uintptr_t dst, uintptr_t src ) = 0;
        
    public:
        uint64_t                m_singleton : 1;
        size_t                  m_capacity;
        size_t                  m_count;
        size_t                  m_refCount;
        uint8_t **              m_components;
        size_t                  m_componentStride;
        ComponentPrototype **   m_prototypes;
        
        std::unordered_map<Entity, uintptr_t> m_entityToIndexMap;
        std::unordered_map<uintptr_t, Entity> m_indexToEntityMap;
    };
    
    
    template< typename __type__, size_t __capacity__ >
    class ComponentArray : public ComponentArrayBase {
    public:
        ComponentArray() {
            m_singleton = 0;
            m_capacity = __capacity__;
            m_count = 0;
            m_refCount = 0;
            m_components = &m_componentArray[ 0 ];
            m_componentStride = sizeof(__type__);
            m_prototypes = &m_prototypeArray[ 0 ];
            
            for( uintptr_t i = 0; i < __capacity__; ++i ) {
                m_componentArray[i] = (void*) &m_componentMemory[i];
            }
        }
        
        virtual ~ComponentArray() {
            
        }
        
    protected:
        
        virtual void CopyInternal( uintptr_t dst, uintptr_t src ) override {
            m_componentMemory[ dst ] = m_componentMemory[ src ];
        }
        
    protected:
        std::array<__type__, __capacity__>              m_componentMemory;
        std::array<void *, __capacity__>                m_componentArray;
        std::array<ComponentPrototype*, __capacity__>   m_prototypeArray;
    };
    
    template< typename __type__ >
    class ComponentSingleton : public ComponentArrayBase {
    public:
        ComponentSingleton() {
            m_singleton = 1;
            m_capacity = 1;
            m_count = 0;
            m_refCount = 0;
            m_components = &m_componentArray[ 0 ];
            m_componentStride = sizeof(__type__);
            m_prototypes = &m_prototypeArray[ 0 ];
            
            for( uintptr_t i = 0; i < m_capacity; ++i ) {
                m_componentArray[i] = (void*) &m_componentMemory[i];
            }
        }
        
        virtual ~ComponentSingleton() {
            
        }
        
    protected:
        
        virtual void CopyInternal( uintptr_t dst, uintptr_t src ) override {
            m_componentMemory[ dst ] = m_componentMemory[ src ];
        }
        
    protected:
        std::array<__type__, 1>              m_componentMemory;
        std::array<void *, 1>                m_componentArray;
        std::array<ComponentPrototype*, 1>   m_prototypeArray;
    };
}


#endif

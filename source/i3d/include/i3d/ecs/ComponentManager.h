//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_COMPONENTMANAGER_H__
#define __I3D_COMPONENTMANAGER_H__

#include "i3d/core/Types.h"
#include "i3d/ecs/EcsTypes.h"
#include "i3d/ecs/ComponentArray.h"

namespace i3d {
    
    class ComponentManager {
    public:
        ComponentManager() {
            m_currType = 0;
        }
        
        ~ComponentManager() {
            
        }
         
        template<typename __type__>
        void RegisterComponentArray( ComponentArrayBase * componentArray ) {
            const char * typeName = typeid(__type__).name();
            XE_ASSERT( m_componentMap.find( typeName ) == m_componentMap.end() );
            
            ComponentType compType = m_currType;
            ++m_currType;
            
            m_componentArrays[ compType ] = componentArray;
            m_componentMap[ typeName ] = compType;
        }
        
        template<typename __type__>
        ComponentType GetComponentType() {
            const char * typeName = typeid(__type__).name();
            XE_ASSERT( m_componentMap.find( typeName ) != m_componentMap.end() );
            return m_componentMap[ typeName ];
        }
        
        template<typename __type__>
        __type__ * AddEntity( Entity ent ) {
            ComponentType compType = GetComponentType<__type__>();
            return m_componentArrays[ compType ]->AddEntity<__type__>( ent );
        }
        
        void RemoveEntity( Entity ent, const EntitySignature & sig  ) {
            ComponentType type = 0;
            for ( auto a : m_componentArrays ) {
                if ( sig[type] != 0 ) {
                    a->RemoveEntity( ent );
                }
                
                ++type;
            }
        }
        
        ComponentArrayBase * GetComponentArray( ComponentType compType ) {
            XE_ASSERT( compType < m_currType );
            return m_componentArrays[ compType ];
        }
        
        template<typename __type__>
        ComponentArrayBase * GetComponentArray() {
            ComponentType compType = GetComponentType<__type__>();
            return GetComponentArray( compType );
        }

    protected:
        std::unordered_map<const char *,  ComponentType> m_componentMap;
        std::array<ComponentArrayBase *, ECS_MAX_COMPONERNTS> m_componentArrays;
        ComponentType m_currType;
    };
}


#endif

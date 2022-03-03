//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_ENTITYMANAGER_H__
#define __I3D_ENTITYMANAGER_H__

#include "i3d/core/Types.h"
#include "i3d/ecs/EcsTypes.h"

namespace i3d {
    
    class EntityManager {
    public:
        EntityManager() {
            for ( Entity entity = 0; entity < ECS_MAX_ENTITIES; ++entity ) {
                m_availableEntities.push(entity);
            }
            
            m_activeCount = 0;
        }
        
        ~EntityManager() {
            
        }
        
        Entity AllocEntity() {
            XE_ASSERT( m_activeCount < ECS_MAX_ENTITIES );
            Entity ent = m_availableEntities.front();
            m_availableEntities.pop();
            
            return ent;
        }
        
        void FreeEntity( Entity ent ) {
            assert( ent < ECS_MAX_ENTITIES );

            m_signatures[ ent ].reset();
            m_availableEntities.push( ent );
            --m_activeCount;
        }
        
        void SetSignature( Entity ent, EntitySignature signature) {
            XE_ASSERT( ent < ECS_MAX_ENTITIES );
            m_signatures[ ent ] = signature;
        }
        
        const EntitySignature & GetSignature( Entity ent ) {
            assert( ent < ECS_MAX_ENTITIES );
            return m_signatures[ ent ];
        }
        
    public:
        std::queue<Entity>          m_availableEntities;
        std::array<EntitySignature, ECS_MAX_ENTITIES> m_signatures;
        size_t                      m_activeCount;
    };
}

#endif 

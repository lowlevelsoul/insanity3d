//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SYSTEM_H__
#define __I3D_SYSTEM_H__

#include "i3d/core/Types.h"
#include "i3d/ecs/EcsTypes.h"
#include <set>

namespace i3d {
    
    class System {
    public:
        std::set<Entity>    m_entities;
    };
    
    
    class SystemManager {
    public:
        SystemManager() {
            
        }
        
        ~SystemManager() {
            
        }
        
        void PublishSystem( System * system, const EntitySignature & sig ) {
            m_systems.push_back( system );
            m_signatures.push_back( sig );
        }
        
        void EntityDestroyed(Entity entity) {
            for (auto s : m_systems) {
                s->m_entities.erase(entity);
            }
        }

        void EntitySignatureChanged( Entity entity, const EntitySignature & entitySignature ) {
            for ( uint32_t i = 0; i < m_systems.size(); ++i ) {
                System * system = m_systems[ i ];
                EntitySignature & systemSignature = m_signatures[ i ];

                if ( ( entitySignature & systemSignature ) == systemSignature ) {
                    system->m_entities.insert(entity);
                }
                else {
                    system->m_entities.erase(entity);
                }
            }
        }
        
    protected:
        stl::Vector<EntitySignature>::type    m_signatures;
        stl::Vector<System *>::type           m_systems;
    };
}


#endif

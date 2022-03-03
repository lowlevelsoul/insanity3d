//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_ECSMANAGER_H__
#define __I3D_ECSMANAGER_H__

#include "i3d/ecs/ComponentArray.h"
#include "i3d/ecs/ComponentManager.h"
#include "i3d/ecs/System.h"
#include "i3d/ecs/EntityManager.h"

namespace i3d {
    
    class EcsSystem {
    public:
        EcsSystem();
        
        ~EcsSystem();
        
        Entity AllocEntity() {
            return m_entities.AllocEntity();
        }
        
        void FreeEntity( Entity ent );
        
        void PublishSystem( System * system, const EntitySignature & sig );
        
        template<typename __type__>
        void PublishComponent(ComponentArrayBase * componentArray ) {
            m_components.RegisterComponentArray<__type__>( componentArray );            
        }
        
        void EntitySignatureChanged( Entity ent, const EntitySignature & sig ) {
            m_systems.EntitySignatureChanged( ent, sig );
        }
        
        template<typename __type__>
        __type__ * AddEntityComponent( Entity ent, bool sigSystemChanged = true  ) {
            EntitySignature sig = m_entities.GetSignature( ent );
            
            ComponentType type = m_components.GetComponentType<__type__>();
            sig[type] = 1;
            
            m_entities.SetSignature( ent, sig );
            
            if ( sigSystemChanged == true ) {
                m_systems.EntitySignatureChanged( ent, sig );
            }
        }
        
    protected:
        EntityManager           m_entities;
        ComponentManager        m_components;
        SystemManager           m_systems;
    };
}

extern i3d::EcsSystem * ecs;

#endif

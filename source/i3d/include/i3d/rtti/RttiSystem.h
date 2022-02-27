//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTISYSTEM_H__
#define __I3D_RTTISYSTEM_H__

#include "i3d/core/Types.h"
#include "i3d/rtti/RttiType.h"
#include "i3d/rtti/RttiObject.h"

namespace i3d {
    
    class RttiType;
    class RttiObject;
    struct RttiPropDef;

    class RttiSystem {
    public:
        virtual ~RttiSystem() {
            // Empty - does nothing
        }
        
        virtual RttiType * Find( const char * name ) = 0;
        
        virtual RttiType * Find( uint64_t nameHash ) = 0;

        virtual RttiObject * LoadObject( const char * path ) = 0;

        template<typename __type__>
        __type__ * LoadObject( const char * path ) {
            RttiObject * loadedObject = LoadObject( path );
            if ( loadedObject == nullptr ) {
                return nullptr;
            }

            __type__ * typedObject = loadedObject->SafeCast<__type__>();
            return typedObject;
        }

        virtual void PublishTypes( RttiType * listStart ) = 0;
        
        RttiObject* Create( const char * name ) {
            RttiType * t = Find(name);
            if (t == nullptr) {
                return nullptr;
            }
            return t->Create();
        }
        
        RttiObject* Create( uint64_t nameHash ) {
            RttiType * t = Find(nameHash);
            if (t == nullptr) {
                return nullptr;
            }
            return t->Create();
        }
        
        template<typename __type__>
        inline __type__* CreateSafe(const char * name) {
            RttiType* t = Find(name);
            if ((t == nullptr) || ((t->IsOfType(__type__::GetClassType())) == false)) {
                return nullptr;
            }
            
            return (__type__ *) t->Create();
        }
        
        template<typename __type__>
        inline __type__* CreateSafe(uint64_t nameHash) {
            RttiType* t = Find(nameHash);
            if ((t == nullptr) || ((t->IsOfType(__type__::GetClassType())) == false)) {
                return nullptr;
            }
            
            return (__type__ *) t->Create();
        }
    };
}

extern i3d::RttiSystem * rtti;

#endif 

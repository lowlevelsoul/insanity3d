//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTISYSTEM_LOCAL_H__
#define __I3D_RTTISYSTEM_LOCAL_H__

#include "i3d/rtti//RttiType.h"
#include "i3d/rtti/RttiSystem.h"

namespace i3d {
    
    class RttiType;
    class RttiObject;
    struct RttiPropDef;

    class RttiSystemLocal : public RttiSystem {
    public:
        
        RttiSystemLocal();
        
        virtual ~RttiSystemLocal();
        
        void Initialise();
        
        void CountClasses( size_t& classCount );
        
        void PublishType(RttiType * type);
        
        RttiType* Find(const char* name) override;
        
        RttiType* Find(uint64_t nameHash) override;

        virtual void PublishTypes( RttiType * listStart ) override;

        virtual RttiObject * LoadObject( const char * path ) override;
           
    protected:
        typedef stl::Map<uint64_t, RttiType *> type_map_t;
        type_map_t::type    m_types;
    };
}

#endif 

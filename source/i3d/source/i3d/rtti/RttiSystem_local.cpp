//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/core/Hash.h"
#include "i3d/rtti/RttiType.h"
#include "i3d/rtti/RttiSystem_local.h"
#include "i3d/rtti/RttiLoader.h"

i3d::RttiSystem * rtti = nullptr;
i3d::RttiSystemLocal * rttiLocal = nullptr;

namespace i3d {

    //======================================================================================================================
    void RttiCreate() {
        if ( rttiLocal != nullptr ) {
            return;
        }

        rttiLocal = new RttiSystemLocal;
        rtti = rttiLocal;
        
        rttiLocal->Initialise();
    }

    //======================================================================================================================
    void RttiDestroy() {
        SAFE_DELETE( rttiLocal );
        rtti = nullptr;
    }

    //======================================================================================================================
    RttiSystemLocal::RttiSystemLocal() {
    }

    //======================================================================================================================
    void RttiSystemLocal::PublishTypes( RttiType * listStart ) {
        for(RttiType* t = listStart; t != nullptr; t = t->m_initNext) {
            PublishType(t);
        }
    }

    //======================================================================================================================
    RttiSystemLocal::~RttiSystemLocal() {

    }

    //======================================================================================================================
    void RttiSystemLocal::Initialise() {
        size_t classCount;
        CountClasses(classCount);
        
        // Publish all of the types to the rtti system
        for(RttiType* t = RttiType::INIT_LIST_HEAD; t != nullptr; t = t->m_initNext) {
            PublishType(t);
        }
    }
        
    //======================================================================================================================
    void RttiSystemLocal::CountClasses(size_t& classCount) {
        classCount = 0;
        
        for(RttiType* t = RttiType::INIT_LIST_HEAD; t != nullptr; t = t->m_initNext) {
            ++classCount;
        }
    }

    //======================================================================================================================
    void RttiSystemLocal::PublishType(RttiType* type) {
        
        uint64_t hash = type->m_nameId; //i3d::fh64::CalcFromString(type->m_name);
        m_types[ hash ] = type;
        
        type->PublishProps();
    }

    //======================================================================================================================
    RttiType* RttiSystemLocal::Find(const char* name) {
        uint64_t hash = fh64::CalcFromString(name);
        return Find(hash);
    }

    //======================================================================================================================
    RttiType* RttiSystemLocal::Find(uint64_t hash) {
        
        type_map_t::iterator findIt = m_types.find( hash );
        if ( findIt == m_types.end() ) {
            return nullptr;
        }
        
        return findIt->second;
    }

    //======================================================================================================================
    RttiObject * RttiSystemLocal::LoadObject( const char * path ) {
        RttiLoader loader;
        loader.Load( path );

        return loader.m_object;
    }
}

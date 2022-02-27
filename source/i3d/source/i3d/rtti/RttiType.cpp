//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/core/Hash.h"
#include "i3d/core/Sys.h"
#include "i3d/rtti/RttiType.h"
#include "i3d/rtti/RttiSystem.h"

namespace i3d {
    
    RttiType*    RttiType::INIT_LIST_HEAD = nullptr;
    RttiType*    RttiType::INIT_LIST_TAIL= nullptr;

    //======================================================================================================================
    RttiType::RttiType(const char* name, RttiType* super, RttiObjectFactory factory, const RttiPropDef* props) {
        m_name = name;
        m_super = super;
        m_factory = factory;
        m_typeProps = props;
        m_nameId = fh64::CalcFromString(name);
        m_props = nullptr;
        
        if (INIT_LIST_HEAD == nullptr) {
            INIT_LIST_HEAD = this;
        }
        else {
            INIT_LIST_TAIL->m_initNext = this;
        }
        
        m_initNext = nullptr;
        INIT_LIST_TAIL = this;
    }

    //======================================================================================================================
    RttiObject* RttiType::Create() {
        return m_factory();
    }

    //======================================================================================================================
    const RttiPropDef* RttiType::FindProp(const char* name) {
        uint64_t hash = fh64::CalcFromString(name);
        return FindProp(hash);
    }

    //======================================================================================================================
    const RttiPropDef* RttiType::FindProp(uint64_t nameHash) {
        if (m_props == nullptr) {
            return nullptr;
        }
        
        if (m_props->m_propHashes.empty() == true) {
            return nullptr;
        }
        
        int32_t index = -1;
        bool found = Sys::BinarySearch( index, nameHash, &m_props->m_propHashes[0] , m_props->m_propHashes.size());
        if ( found == false ) {
            return nullptr;
        }
        
        RttiPropDef * propDef = &m_props->m_props[ m_props->m_propMap[ index ] ];
        return propDef;
    }

    //======================================================================================================================
    bool RttiType::IsOfType(const RttiType* type) const {
        
        const RttiType* curr = this;
        
        while(curr != nullptr) {
            
            if (type == curr) {
                return true;
            }
            
            curr = curr->m_super;
        }
        
        return false;
    }

    //======================================================================================================================
    void RttiType::PublishProps() {
        
        RttiType * currType = this;
        
        while(currType != nullptr) {
        
            for(const RttiPropDef* p = currType->m_typeProps; p->m_type != RTTI_PROP_TYPE_NULL; ++p) {
                
                uint64_t hash = fh64::CalcFromString(p->m_name);
                
                if ( m_props == nullptr ) {
                    
                    m_props = new PropInfo;
                    
                    m_props->m_props.push_back(*p);
                    m_props->m_propHashes.push_back(hash);
                    m_props->m_propMap.push_back(0);
                }
                else {
                    int32_t insertIndex = -1;
                    bool found = Sys::BinarySearch<uint64_t, int32_t>( insertIndex, hash, &m_props->m_propHashes[0], m_props->m_propHashes.size() );
                    assert(found == false);
                    
                    int32_t propIndex = (int32_t) m_props->m_props.size();
                    m_props->m_props.push_back(*p);
                    m_props->m_propHashes.insert( m_props->m_propHashes.begin() + insertIndex, hash );
                    m_props->m_propMap.insert( m_props->m_propMap.begin() + insertIndex, propIndex );
                }
            }
            
            currType = currType->GetSuper();
        }
    }
}

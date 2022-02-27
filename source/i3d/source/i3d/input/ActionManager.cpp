//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/ActionManager.h"

xe::ActionManager * action = nullptr;

namespace xe {
    
    //======================================================================================================================
    void CreateActionManager() {
        if ( action != nullptr ) {
            return;
        }
        
        action = new ActionManager;
    }
    
    //======================================================================================================================
    void DestroyActionManager() {
        SAFE_DELETE( action );
    }

    //======================================================================================================================
    ActionManager::ActionManager() {
        
    }
    
    //======================================================================================================================
    ActionManager::~ActionManager() {
        
    }
    
    //======================================================================================================================
    void ActionManager::BeginFrame() {
        
        // Store the action vales from the last frame
        for ( auto ap : m_profiles ) {
            for ( auto a : ap->m_actions ) {
                a->m_valueLastFrame = a->m_value;
            }
        }
        
    }
    
    //======================================================================================================================
    ActionProfile * ActionManager::AddProfile( const char * name ) {
        ActionProfile * profile = new ActionProfile;
        profile->m_name = name;
        AddProfile( profile );
        
        return profile;
    }
    
    //======================================================================================================================
    void ActionManager::AddProfile( ActionProfile * profile ) {
        XE_ASSERT( FindProfle( profile->m_name.GetCStr() ) == nullptr );
        
        uint64_t nameHash = MakeProfileNameHash( profile->m_name.GetCStr() );
        int32_t insertIndex = -1;
        m_nameHashArray.SortedFind( insertIndex, nameHash );
        
        m_nameHashArray.Insert( m_nameHashArray.Begin() + insertIndex, nameHash );
        m_profiles.Insert( m_profiles.Begin() + insertIndex, profile );
    }
    
    //======================================================================================================================
    ActionProfile * ActionManager::FindProfle( const char * name ) {
        
        uint64_t nameHash = MakeProfileNameHash( name );
        int32_t index = -1;
        
        bool found = m_nameHashArray.SortedFind( index, nameHash );
        if ( found == false ) {
            return nullptr;
        }
        
        return m_profiles[ index ];
    }
    
    //======================================================================================================================
    uint64_t ActionManager::MakeProfileNameHash( const char * name ) {
        xe::String tmp = name;
        std::transform( tmp.begin(), tmp.end(), tmp.begin(), std::tolower);
        
        return fh64::CalcFromString( name );
    }
    
    //======================================================================================================================
    void ActionManager::BroadcastAction( Key key, float value ) {
        for ( auto profile : m_profiles ) {
            Action * action = profile->FindActionForKey( key );
            if ( action != nullptr ) {
                action->SetValue( value );                
            }
        }
    }

}

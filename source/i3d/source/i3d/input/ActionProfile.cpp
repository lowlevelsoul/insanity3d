//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/ActionProfile.h"

namespace xe {
    
    RTTI_CLASS_BEGIN( Action )
        RTTI_PROP( STRING, "name", m_name )
    RTTI_CLASS_END( Action )
    
    RTTI_CLASS_BEGIN( ActionProfile )
        RTTI_PROP( STRING,            "name",         m_name )
        RTTI_PROP_ARRAY( OBJECT_REFPTR,     "actions",      m_actions )
    RTTI_CLASS_END( ActionProfile )

    //======================================================================================================================
    Action::Action() {
        m_value = 0;
        m_valueLastFrame = 0;
    }
    
    //======================================================================================================================
    Action::~Action() {
        
    }
    
    //======================================================================================================================
    ActionProfile::ActionProfile() {
        
    }
    
    //======================================================================================================================
    ActionProfile::~ActionProfile() {
        
    }
    
    //======================================================================================================================
    void ActionProfile::AddAction( const char * name ) {
        XE_ASSERT( FindAction( name ) == nullptr );     // Ensure action does not exist
        
        Action * action = new Action;
        action->m_name = name;
        
        uint32_t actionIndex = ( uint32_t) m_actions.GetSize();
        m_actions.PushBack( action );
        
        uint64_t nameHash = CalcActionNameHash( name );
        
        int32_t insertIndex;
        m_actionHashArray.SortedFind( insertIndex, nameHash );
        
        m_actionHashArray.Insert( m_actionHashArray.Begin() + insertIndex, nameHash );
        m_actionHashMap.Insert( m_actionHashMap.Begin() + insertIndex, actionIndex );
    }
    
    //======================================================================================================================
    Action * ActionProfile::FindAction( const char * name ) {
        uint64_t nameHash = CalcActionNameHash( name );
        int32_t index = -1;
        bool found = m_actionHashArray.SortedFind( index, nameHash );
        if ( found == false ) {
            return nullptr;
        }
        
        return m_actions[ m_actionHashMap[ index ] ];
    }
    
    //======================================================================================================================
    bool ActionProfile::FindActionIndex( int32_t & index, const char * name ) {
        uint64_t nameHash = CalcActionNameHash( name );
        return m_actionHashArray.SortedFind( index, nameHash );
    }

    //======================================================================================================================
    void ActionProfile::MapInputKey( Key key, const char * actionName ) {
        
        Action * action = FindAction( actionName );
        XE_ASSERT( action != nullptr );
        
        int32_t keyIndex;
        bool found = m_actionKeyArray.SortedFind( keyIndex, key);
        if ( found == true ) {
            // We already have a key mapped to an action, so
            // remap it by replacing the action
            m_actionKeyMap[ keyIndex ] = action;
        }
        else {
            // This kep doesn't exist in the action map. So add it.
            m_actionKeyArray.Insert( m_actionKeyArray.Begin() + keyIndex , key );
            m_actionKeyMap.Insert( m_actionKeyMap.Begin() + keyIndex, action );
        }
    }
    
    //======================================================================================================================
    uint64_t ActionProfile::CalcActionNameHash( const char * name ) {
        xe::String tmp = name;
        std::transform( tmp.begin(), tmp.end(), tmp.begin(), std::tolower );
        
        uint64_t hash = xe::fh64::CalcFromString( &tmp[ 0 ] );
        return hash;
    }
    
    //======================================================================================================================
    Action * ActionProfile::FindActionForKey( Key key ) {
        int32_t index = -1;
        bool found = m_actionKeyArray.SortedFind( index, key);
        if ( found == false ) {
            return nullptr;
        }
        
        return m_actionKeyMap[ index ];
    }
    
    //======================================================================================================================
    bool ActionProfile::ReadComplete( const char * path ) {
        
        uint32_t actionIndex = 0;
        for ( auto a : m_actions ) {
            uint64_t nameHash = CalcActionNameHash( a->m_name.GetCStr() );
            
            int32_t insertIndex;
            bool found = m_actionHashArray.SortedFind( insertIndex, nameHash );
            XE_ASSERT( found == false );
            
            m_actionHashArray.Insert( m_actionHashArray.Begin() + insertIndex, nameHash );
            m_actionHashMap.Insert( m_actionHashMap.Begin() + insertIndex, actionIndex );
            
            ++actionIndex;
        }
        
        return true;
    }
}

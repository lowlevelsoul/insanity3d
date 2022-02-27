//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_ACTIONPROFILE_H__
#define __XENGINE_ACTIONPROFILE_H__

#include "xe/input/InputTypes.h"

namespace xe {
    
    class Action : public RttiObject {
    public:
        RTTI_CLASS_DECLARE( Action, RttiObject )
        
        Action();
        
        virtual ~Action();
        
        bool IsPressed() const { return ( m_value != 0 ); }
        
        bool IsPressedDebounced() const { return ( m_value != 0) && ( m_valueLastFrame != 0); }
        
        float Getvalue() const { return m_value; }
        
        void SetValue( float value ) {
            m_value = value;
        }
        
    public:
        xe::String      m_name;             ///< The action name
        float           m_value;            ///< The value of the action for this frame
        float           m_valueLastFrame;   ///<
    };
    
    class ActionProfile : public RttiObject {
    public:
        RTTI_CLASS_DECLARE( ActionProfile, RttiObject )
        
        ActionProfile();
        
        virtual ~ActionProfile();
        
        void AddAction( const char * name );
        
        Action * FindAction( const char * name );
        
        void MapInputKey( Key key, const char * action );
        
        Action * FindActionForKey( Key key );
        
        virtual bool ReadComplete( const char * path ) override;
                
    protected:
        
       
        
        bool FindActionIndex( int32_t & index, const char * name );
        
        bool FindInputKey( int32_t & index, Key key ) const;
        
        uint64_t CalcActionNameHash( const char * name );
        
    public:
        xe::String                      m_name;
        xe::Array<Action::ref_ptr_t>    m_actions;
        xe::Array<uint64_t>             m_actionHashArray;
        xe::Array<uint32_t>             m_actionHashMap;
        
        xe::Array<Key>                  m_actionKeyArray;
        xe::Array<Action*>              m_actionKeyMap;
    };
}

#endif

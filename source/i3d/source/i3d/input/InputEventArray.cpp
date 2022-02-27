//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/InputEventArray.h"

namespace xe {

    //======================================================================================================================
    InputEventArray::InputEventArray() {
        m_events.Reserve( 256 );
    }
    
    //======================================================================================================================
    InputEventArray::~InputEventArray() {
        
    }
    
    //======================================================================================================================
    bool InputEventArray::PushKeyEvent( Key key, uint32_t mod, float value ) {
        InputEvent ev;
        ev.m_type = xe::InputEventType_Key;
        ev.m_data.m_key.m_key = key;
        ev.m_data.m_key.m_value = value;
        ev.m_data.m_key.m_modifiers = mod;
        
        m_events.PushBack( ev );
        
        return true;
    }    
}

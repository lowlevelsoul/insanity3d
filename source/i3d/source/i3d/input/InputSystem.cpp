//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/InputSystem.h"

namespace xe {

    //======================================================================================================================
    InputSystem::InputSystem() {
        m_destEvents = &m_events[0];
        m_srcEvents = &m_events[1];
    }

    //======================================================================================================================
    InputSystem::~InputSystem() {
        
    }
    
    //======================================================================================================================
    void InputSystem::Think() {
        xe::ScopedLock lock( m_mutex );
        
        // Switch the input / output event arrays
        InputEventArray * tmp = m_srcEvents;
        m_srcEvents = m_destEvents;
        m_destEvents = tmp;
        
        m_destEvents->Reset();
    }
}

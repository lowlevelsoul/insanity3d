//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_INPUTSYSTEM_H__
#define __I3D_INPUTSYSTEM_H__

#include "i3d/input/InputEventArray.h"

namespace i3d {
    
    class InputSystem {
    public:
        InputSystem();
        
        ~InputSystem();
        
        void Think();
        
        InputEventArray * GetEvents() {
            return m_srcEvents;
        }
        
    protected:
        i3d::Mutex           m_mutex;
        InputEventArray     m_events[2];
        InputEventArray *   m_destEvents;
        InputEventArray *   m_srcEvents;
    };
}

extern i3d::InputSystem * input;

#endif

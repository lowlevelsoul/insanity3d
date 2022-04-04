//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================
 
#ifndef __INPUTSYSTEM_H__
#define __INPUTSYSTEM_H__

#include "i3d/input/InputTypes.h"

namespace i3d {

    class InputSystem {
    public:
        struct EventArray {
            typedef stl::Vector<Event> event_vector;
            event_vector::type      m_events;
            uint32_t                m_eventCount;
            
            void Initialise(size_t capacity) {
                m_events.reserve( capacity );
                m_eventCount = 0;
            }
            
            void Finalise() {
                m_events.clear();
            }
            
            void Reset() {
                m_eventCount = 0;
                m_events.resize( 0 );
            }
        };
        
        virtual ~InputSystem() {
        }
        
        virtual void PushKeyEvent(KEY key, char charPressed, uint32_t modifier, float value) = 0;
        
        virtual void PushEvent(const Event& ev) = 0;
        
        virtual EventArray& SwapEvents() = 0;
        
        virtual void Think(Event*& eventArray, uint32_t& eventCount) = 0;
    };
}

extern i3d::InputSystem * input;

#endif

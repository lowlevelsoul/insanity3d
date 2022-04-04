//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================
 
#ifndef __INPUTSYSTEM_IOS_H__
#define __INPUTSYSTEM_IOS_H__

#include "i3d/input/Input.h"

namespace i3d {

    class InputSystemIos : public InputSystem {
    public:
        static const size_t EVENT_ARRAY_COUNT = 2;
        static const size_t EVENT_CAPACITY = 256;
        static const size_t KEY_COUNT = 256;
        
        EventArray      m_events[EVENT_ARRAY_COUNT];
        KEY             m_keyTranslate[KEY_COUNT];          ///< Internal table for translating key event keys to internal key values
        uint32_t        m_currEvents;
        uint32_t        m_prevEvents;
        std::mutex      m_mutex;
        
        InputSystemIos();
        
        virtual ~InputSystemIos();
        
        uint32_t TranslateModifier(uint32_t flags);
        
        void OnTouchBegin(UITouch * event);
        
        void OnTouchEnd(UITouch * event);
        
        void OnTouchMove(UITouch * event);
        
        virtual void PushKeyEvent(KEY key, char charPressed, uint32_t modifier, float value) override;
        
        virtual void PushEvent(const Event& ev) override;
        
        virtual EventArray& SwapEvents() override;
        
        virtual void Think(Event*& eventArray, uint32_t& eventCount) override;
    };
}

extern i3d::InputSystemIos * inputLocal;

#endif

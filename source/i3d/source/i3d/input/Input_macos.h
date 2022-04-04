//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================
 
#ifndef __INPUTSYSTEM_LOCAL_H__
#define __INPUTSYSTEM_LOCAL_H__

#include "input/Input.h"

class InputSystemLocal : public InputSystem {
public:
    static const size_t EVENT_ARRAY_COUNT = 2;
    static const size_t EVENT_CAPACITY = 256;
    static const size_t KEY_COUNT = 256;
    
    EventArray      m_events[EVENT_ARRAY_COUNT];
    KEY             m_keyTranslate[KEY_COUNT];          ///< Internal table for translating key event keys to internal key values
    uint32_t        m_currEvents;
    uint32_t        m_prevEvents;
    std::mutex      m_mutex;
    
    InputSystemLocal();
    
    virtual ~InputSystemLocal();
    
    uint32_t TranslateModifier(uint32_t flags);
    
    void InitKeyTranslate();
    
    virtual void PushKeyEvent(KEY key, char charPressed, uint32_t modifier, float value) override;
    
    virtual void PushEvent(const Event& ev) override;
    
    virtual EventArray& SwapEvents() override;
    
    virtual void Think(Event*& eventArray, uint32_t& eventCount) override;
};

extern InputSystemLocal* inputSystem;

#endif

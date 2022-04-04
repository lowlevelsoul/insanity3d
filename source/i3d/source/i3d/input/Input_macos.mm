//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "core/HeapArray.h"
#include "input/Input_macos.h"
#include "Engine.h"
#include <mutex>

static uint8_t inputSystemMem[sizeof(InputSystemLocal)];
InputSystemLocal* inputSystem = nullptr;

//======================================================================================================================
void InputCreate() {
    assert(inputSystem == nullptr);
    inputSystem = new(inputSystemMem) InputSystemLocal;
    engine.input = inputSystem;
}

//======================================================================================================================
void InputDestroy() {
    assert(inputSystem != nullptr);
    inputSystem->~InputSystemLocal();
    inputSystem = nullptr;
    engine.input = nullptr;
}

//======================================================================================================================
void InputSystemLocal::InitKeyTranslate() {
    
    memset(m_keyTranslate, 0, sizeof(m_keyTranslate));
    
    m_keyTranslate[27]             = KEY_Esc;
    m_keyTranslate[uint8_t('\r')]  = KEY_Return;
    m_keyTranslate[uint8_t('\t')]  = KEY_Tab;
    m_keyTranslate[127]            = KEY_Backspace;
    m_keyTranslate[uint8_t(' ')]   = KEY_Space;

    m_keyTranslate[uint8_t('+')]   =
    m_keyTranslate[uint8_t('=')]   = KEY_Plus;
    m_keyTranslate[uint8_t('_')]   =
    m_keyTranslate[uint8_t('-')]   = KEY_Minus;

    m_keyTranslate[uint8_t('~')]   =
    m_keyTranslate[uint8_t('`')]   = KEY_Tilde;

    m_keyTranslate[uint8_t(':')]   =
    m_keyTranslate[uint8_t(';')]   = KEY_Semicolon;
    m_keyTranslate[uint8_t('"')]   =
    m_keyTranslate[uint8_t('\'')]  = KEY_Quote;

    m_keyTranslate[uint8_t('{')]   =
    m_keyTranslate[uint8_t('[')]   = KEY_LeftBracket;
    m_keyTranslate[uint8_t('}')]   =
    m_keyTranslate[uint8_t(']')]   = KEY_RightBracket;

    m_keyTranslate[uint8_t('<')]   =
    m_keyTranslate[uint8_t(',')]   = KEY_Comma;
    m_keyTranslate[uint8_t('>')]   =
    m_keyTranslate[uint8_t('.')]   = KEY_Period;
    m_keyTranslate[uint8_t('?')]   =
    m_keyTranslate[uint8_t('/')]   = KEY_Slash;
    m_keyTranslate[uint8_t('|')]   =
    m_keyTranslate[uint8_t('\\')]  = KEY_Backslash;

    m_keyTranslate[uint8_t('0')]   = KEY_0;
    m_keyTranslate[uint8_t('1')]   = KEY_1;
    m_keyTranslate[uint8_t('2')]   = KEY_2;
    m_keyTranslate[uint8_t('3')]   = KEY_3;
    m_keyTranslate[uint8_t('4')]   = KEY_4;
    m_keyTranslate[uint8_t('5')]   = KEY_5;
    m_keyTranslate[uint8_t('6')]   = KEY_6;
    m_keyTranslate[uint8_t('7')]   = KEY_7;
    m_keyTranslate[uint8_t('8')]   = KEY_8;
    m_keyTranslate[uint8_t('9')]   = KEY_9;

    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        m_keyTranslate[uint8_t(ch)]       =
        m_keyTranslate[uint8_t(ch - ' ')] = KEY(KEY_A + (ch - 'a'));
    }
}
    
//======================================================================================================================
InputSystemLocal::InputSystemLocal() {
    InitKeyTranslate();
    
    m_events[0].Initialise(EVENT_CAPACITY);
    m_events[1].Initialise(EVENT_CAPACITY);
    m_currEvents = 0;
}

//======================================================================================================================
InputSystemLocal::~InputSystemLocal() {
    m_events[0].Finalise();
    m_events[1].Finalise();
}

//======================================================================================================================
void InputSystemLocal::PushEvent(const Event& ev) {
    m_mutex.lock();
    EventArray& events = m_events[m_currEvents];
        
    if (events.m_eventCount < EVENT_CAPACITY) {
        events.m_events[events.m_eventCount] = ev;
        ++events.m_eventCount;
    }
    
    m_mutex.unlock();
}

//======================================================================================================================
void InputSystemLocal::PushKeyEvent(KEY key, char charPressed, uint32_t modifier, float value) {
    Event ev;
    
    ev.m_type = EVENT_TYPE_KEY;
    ev.m_event.m_key.m_key = key;
    ev.m_event.m_key.m_char = charPressed;
    ev.m_event.m_key.m_modifier = modifier;
    ev.m_event.m_key.m_value = value;
    
    PushEvent(ev);
}

//======================================================================================================================
InputSystem::EventArray& InputSystemLocal::SwapEvents() {
    m_mutex.lock();
    
    EventArray& prevEvents = m_events[m_currEvents];
    m_currEvents = (m_currEvents + 1) % (uint32_t) EVENT_ARRAY_COUNT;
    m_events[m_currEvents].Reset();
    
    m_mutex.unlock();
    
    return prevEvents;
}

//======================================================================================================================
void InputSystemLocal::Think(Event*& eventArray, uint32_t& eventCount) {
    EventArray& events = SwapEvents();
    
    eventArray = &events.m_events[0];
    eventCount = events.m_eventCount;
}

//======================================================================================================================
uint32_t InputSystemLocal::TranslateModifier(uint32_t flags) {
    
    uint32_t mod = 0
        | ((0 != (flags & NX_DEVICELSHIFTKEYMASK ) ) ? MODIFIER_LeftShift  : 0)
        | ((0 != (flags & NX_DEVICERSHIFTKEYMASK ) ) ? MODIFIER_RightShift : 0)
        | ((0 != (flags & NX_DEVICELALTKEYMASK ) )   ? MODIFIER_LeftAlt    : 0)
        | ((0 != (flags & NX_DEVICERALTKEYMASK ) )   ? MODIFIER_RightAlt   : 0)
        | ((0 != (flags & NX_DEVICELCTLKEYMASK ) )   ? MODIFIER_LeftCtrl   : 0)
        | ((0 != (flags & NX_DEVICERCTLKEYMASK ) )   ? MODIFIER_RightCtrl  : 0)
        | ((0 != (flags & NX_DEVICELCMDKEYMASK) )    ? MODIFIER_LeftMeta   : 0)
        | ((0 != (flags & NX_DEVICERCMDKEYMASK) )    ? MODIFIER_RightMeta  : 0);
    
    return mod;
}

//======================================================================================================================
KEY InputSystemLocal::TranslateKey(NSEvent* event, uint8_t& specialKeys, uint8_t& pressedChar)
{
    NSString* key = [event charactersIgnoringModifiers];
    unichar keyChar = 0;
    if ([key length] == 0)
    {
        return KEY_None;
    }

    keyChar = [key characterAtIndex:0];
    pressedChar = (uint8_t)keyChar;

    int keyCode = keyChar;
    specialKeys = TranslateModifier(uint32_t([event modifierFlags]));

    // if this is a unhandled key just return None
    if (keyCode < 256) {
        return m_keyTranslate[keyCode];
    }

    switch (keyCode) {
        case NSF1FunctionKey:  return KEY_F1;
        case NSF2FunctionKey:  return KEY_F2;
        case NSF3FunctionKey:  return KEY_F3;
        case NSF4FunctionKey:  return KEY_F4;
        case NSF5FunctionKey:  return KEY_F5;
        case NSF6FunctionKey:  return KEY_F6;
        case NSF7FunctionKey:  return KEY_F7;
        case NSF8FunctionKey:  return KEY_F8;
        case NSF9FunctionKey:  return KEY_F9;
        case NSF10FunctionKey: return KEY_F10;
        case NSF11FunctionKey: return KEY_F11;
        case NSF12FunctionKey: return KEY_F12;

        case NSLeftArrowFunctionKey:   return KEY_Left;
        case NSRightArrowFunctionKey:  return KEY_Right;
        case NSUpArrowFunctionKey:     return KEY_Up;
        case NSDownArrowFunctionKey:   return KEY_Down;

        case NSPageUpFunctionKey:      return KEY_PageUp;
        case NSPageDownFunctionKey:    return KEY_PageDown;
        case NSHomeFunctionKey:        return KEY_Home;
        case NSEndFunctionKey:         return KEY_End;

        case NSPrintScreenFunctionKey: return KEY_Print;
    }

    return KEY_None;
}

//======================================================================================================================
void InputSystemLocal::OnKeyDown(NSEvent* event) {
    uint8_t modifier = 0;
    uint8_t pressedChar = 0;
    
    KEY key = TranslateKey(event, modifier, pressedChar);
    if (key != KEY_None) {
       PushKeyEvent(key, pressedChar, modifier, 1.0f);
    }
}

//======================================================================================================================
void InputSystemLocal::OnKeyUp(NSEvent* event) {
    uint8_t modifier = 0;
    uint8_t pressedChar = 0;
    
    KEY key = TranslateKey(event, modifier, pressedChar);
    if (key != KEY_None) {
        PushKeyEvent(key, pressedChar, modifier, 0.0f);        
    }
}

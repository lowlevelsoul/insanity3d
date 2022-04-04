//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __INPUTTYPES_H__
#define __INPUTTYPES_H__

namespace i3d {
    
    enum EVENT_TYPE {
        EVENT_TYPE_NONE=0,
        EVENT_TYPE_KEY,
        EVENT_TYPE_TOUCH,
        EVENT_TYPE_COUNT,
    };

    enum MODIFIER
    {
        MODIFIER_None       = 0,
        MODIFIER_LeftAlt    = 0x01,
        MODIFIER_RightAlt   = 0x02,
        MODIFIER_LeftCtrl   = 0x04,
        MODIFIER_RightCtrl  = 0x08,
        MODIFIER_LeftShift  = 0x10,
        MODIFIER_RightShift = 0x20,
        MODIFIER_LeftMeta   = 0x40,
        MODIFIER_RightMeta  = 0x80,
    };

    enum KEY {
        KEY_None = 0,
        KEY_Esc,
        KEY_Return,
        KEY_Tab,
        KEY_Space,
        KEY_Backspace,
        KEY_Up,
        KEY_Down,
        KEY_Left,
        KEY_Right,
        KEY_Insert,
        KEY_Delete,
        KEY_Home,
        KEY_End,
        KEY_PageUp,
        KEY_PageDown,
        KEY_Print,
        KEY_Plus,
        KEY_Minus,
        KEY_LeftBracket,
        KEY_RightBracket,
        KEY_Semicolon,
        KEY_Quote,
        KEY_Comma,
        KEY_Period,
        KEY_Slash,
        KEY_Backslash,
        KEY_Tilde,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_NumPad0,
        KEY_NumPad1,
        KEY_NumPad2,
        KEY_NumPad3,
        KEY_NumPad4,
        KEY_NumPad5,
        KEY_NumPad6,
        KEY_NumPad7,
        KEY_NumPad8,
        KEY_NumPad9,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        
        KEY_GamepadA,
        KEY_GamepadB,
        KEY_GamepadX,
        KEY_GamepadY,
        KEY_GamepadThumbL,
        KEY_GamepadThumbR,
        KEY_GamepadShoulderL,
        KEY_GamepadShoulderR,
        KEY_GamepadUp,
        KEY_GamepadDown,
        KEY_GamepadLeft,
        KEY_GamepadRight,
        KEY_GamepadBack,
        KEY_GamepadStart,
        KEY_GamepadGuide,
        
        KEY_Count
    };

    struct KeyEvent {
        KEY         m_key;
        char        m_char;
        uint32_t    m_modifier;
        float       m_value;
        
        void Set(KEY key, char charPressed, uint32_t modifier, float value ) {
            m_key = key;
            m_char = charPressed;
            m_modifier = modifier;
            m_value = value;
        }
    };

    struct TouchEvent {
        enum TYPE {
            TYPE_BEGIN=0,
            TYPE_END,
            TYPE_MOVED,
        };
        TYPE            m_type;
        uint32_t        m_id;
        float           m_x;
        float           m_y;
    };

    struct Event {
        EVENT_TYPE   m_type;
        union {
            KeyEvent    m_key;
            TouchEvent  m_touch;
        } m_event;
    };
}

#endif

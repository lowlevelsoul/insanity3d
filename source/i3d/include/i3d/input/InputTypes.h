//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_INPUTTYPES_H__
#define __I3D_INPUTTYPES_H__

#include "i3d/core/Types.h"

namespace i3d {

    enum KeyModifier
    {
        KeyModifier_None       = 0,
        KeyModifier_LeftAlt    = 0x01,
        KeyModifier_RightAlt   = 0x02,
        KeyModifier_LeftCtrl   = 0x04,
        KeyModifier_RightCtrl  = 0x08,
        KeyModifier_LeftShift  = 0x10,
        KeyModifier_RightShift = 0x20,
        KeyModifier_LeftMeta   = 0x40,
        KeyModifier_RightMeta  = 0x80,
    };

    enum Key
    {
        Key_None = 0,
        Key_Esc,
        Key_Return,
        Key_Tab,
        Key_Space,
        Key_Backspace,
        Key_Up,
        Key_Down,
        Key_Left,
        Key_Right,
        Key_Insert,
        Key_Delete,
        Key_Home,
        Key_End,
        Key_PageUp,
        Key_PageDown,
        Key_Print,
        Key_Plus,
        Key_Minus,
        Key_LeftBracket,
        Key_RightBracket,
        Key_Semicolon,
        Key_Quote,
        Key_Comma,
        Key_Period,
        Key_Slash,
        Key_Backslash,
        Key_Tilde,
        Key_F1,
        Key_F2,
        Key_F3,
        Key_F4,
        Key_F5,
        Key_F6,
        Key_F7,
        Key_F8,
        Key_F9,
        Key_F10,
        Key_F11,
        Key_F12,
        Key_NumPad0,
        Key_NumPad1,
        Key_NumPad2,
        Key_NumPad3,
        Key_NumPad4,
        Key_NumPad5,
        Key_NumPad6,
        Key_NumPad7,
        Key_NumPad8,
        Key_NumPad9,
        Key_Key0,
        Key_Key1,
        Key_Key2,
        Key_Key3,
        Key_Key4,
        Key_Key5,
        Key_Key6,
        Key_Key7,
        Key_Key8,
        Key_Key9,
        Key_KeyA,
        Key_KeyB,
        Key_KeyC,
        Key_KeyD,
        Key_KeyE,
        Key_KeyF,
        Key_KeyG,
        Key_KeyH,
        Key_KeyI,
        Key_KeyJ,
        Key_KeyK,
        Key_KeyL,
        Key_KeyM,
        Key_KeyN,
        Key_KeyO,
        Key_KeyP,
        Key_KeyQ,
        Key_KeyR,
        Key_KeyS,
        Key_KeyT,
        Key_KeyU,
        Key_KeyV,
        Key_KeyW,
        Key_KeyX,
        Key_KeyY,
        Key_KeyZ,

        Key_GamepadA,
        Key_GamepadB,
        Key_GamepadX,
        Key_GamepadY,
        Key_GamepadThumbL,
        Key_GamepadThumbR,
        Key_GamepadShoulderL,
        Key_GamepadShoulderR,
        Key_GamepadUp,
        Key_GamepadDown,
        Key_GamepadLeft,
        Key_GamepadRight,
        Key_GamepadBack,
        Key_GamepadStart,
        Key_GamepadGuide,
        
        Key_GamepadAxisX,
        Key_GamepadAxisY,
        Key_GamepadAxisZ,
        Key_GamepadAxisW,
        
        Key_MouseButton1,
        Key_MouseButton2,
        Key_MouseButton3,
        Key_MouseMoved,

        Key_Count
    };

    enum InputEventType {
        InputEventType_None = 0,
        InputEventType_Key,
        InputEventType_Count
    };

    struct InputKeyEvent {
        Key         m_key;
        uint32_t    m_modifiers;
        float       m_value;
    };


    struct InputEvent {
        InputEventType       m_type;
        union {
            InputKeyEvent        m_key;
        } m_data;
    };
}


#endif

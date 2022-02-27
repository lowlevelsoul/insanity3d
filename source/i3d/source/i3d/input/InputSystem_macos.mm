//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/InputSystem_macos.h"

xe::InputSystem * input = nullptr;
xe::InputSystemMacos * inputLocal = nullptr;

namespace xe {
    
    void InputCreate() {
        if (input != nullptr ) {
            return;
        }
        
        inputLocal = new InputSystemMacos;
        inputLocal->Initialise();
        input = inputLocal;
    }
    
    //======================================================================================================================
    void InputDestroy() {
        if (input == nullptr ) {
            return;
        }
        
        delete inputLocal;
        inputLocal = nullptr;
        input = nullptr;
    }

    //======================================================================================================================
    InputSystemMacos::InputSystemMacos() {
        
    }
    
    //======================================================================================================================
    InputSystemMacos::~InputSystemMacos() {
        
    }

    //======================================================================================================================
    void InputSystemMacos::Initialise() {
        InitialiseTranslateKey();
    }
    
    //======================================================================================================================
    void InputSystemMacos::InitialiseTranslateKey() {
        memset(m_translateKey, 0, sizeof(m_translateKey));
        
        m_translateKey[27]             = Key_Esc;
        m_translateKey[uint8_t('\r')]  = Key_Return;
        m_translateKey[uint8_t('\t')]  = Key_Tab;
        m_translateKey[127]            = Key_Backspace;
        m_translateKey[uint8_t(' ')]   = Key_Space;

        m_translateKey[uint8_t('+')]   =
        m_translateKey[uint8_t('=')]   = Key_Plus;
        m_translateKey[uint8_t('_')]   =
        m_translateKey[uint8_t('-')]   = Key_Minus;

        m_translateKey[uint8_t('~')]   =
        m_translateKey[uint8_t('`')]   = Key_Tilde;

        m_translateKey[uint8_t(':')]   =
        m_translateKey[uint8_t(';')]   = Key_Semicolon;
        m_translateKey[uint8_t('"')]   =
        m_translateKey[uint8_t('\'')]  = Key_Quote;

        m_translateKey[uint8_t('{')]   =
        m_translateKey[uint8_t('[')]   = Key_LeftBracket;
        m_translateKey[uint8_t('}')]   =
        m_translateKey[uint8_t(']')]   = Key_RightBracket;

        m_translateKey[uint8_t('<')]   =
        m_translateKey[uint8_t(',')]   = Key_Comma;
        m_translateKey[uint8_t('>')]   =
        m_translateKey[uint8_t('.')]   = Key_Period;
        m_translateKey[uint8_t('?')]   =
        m_translateKey[uint8_t('/')]   = Key_Slash;
        m_translateKey[uint8_t('|')]   =
        m_translateKey[uint8_t('\\')]  = Key_Backslash;

        m_translateKey[uint8_t('0')]   = Key_Key0;
        m_translateKey[uint8_t('1')]   = Key_Key1;
        m_translateKey[uint8_t('2')]   = Key_Key2;
        m_translateKey[uint8_t('3')]   = Key_Key3;
        m_translateKey[uint8_t('4')]   = Key_Key4;
        m_translateKey[uint8_t('5')]   = Key_Key5;
        m_translateKey[uint8_t('6')]   = Key_Key6;
        m_translateKey[uint8_t('7')]   = Key_Key7;
        m_translateKey[uint8_t('8')]   = Key_Key8;
        m_translateKey[uint8_t('9')]   = Key_Key9;

        for (char ch = 'a'; ch <= 'z'; ++ch) {
            m_translateKey[uint8_t(ch)]       =
            m_translateKey[uint8_t(ch - ' ')] = Key_KeyA + (ch - 'a');
        }
    }
    
    //======================================================================================================================
    void InputSystemMacos::KeyEvent( NSEvent * ev ) {
        NSEventType eventType = [ ev type ];
        
        float keyValue = ( eventType == NSEventTypeKeyDown ) ? 1.0f : 0.0f;
        
        uint8_t modifiers = 0;
        uint8_t pressedChar[ 4 ];
        Key key = HandleKeyEvent( ev, &modifiers, &pressedChar[0]);
        
        if ( key != Key_None ) {
            xe::ScopedLock lock( m_mutex );
            m_destEvents->PushKeyEvent( key, modifiers, keyValue );
        }
    }
    
    //======================================================================================================================
    Key InputSystemMacos::HandleKeyEvent( NSEvent* event, uint8_t* specialKeys, uint8_t * _pressedChar )
    {
        NSString* key = [event charactersIgnoringModifiers];
        unichar keyChar = 0;
        if ([key length] == 0)
        {
            return Key_None;
        }

        keyChar = [key characterAtIndex:0];
        *_pressedChar = (uint8_t) keyChar;

        int keyCode = keyChar;
        *specialKeys = TranslateModifiers(int([event modifierFlags]));

        // if this is a unhandled key just return None
        if (keyCode < 256) {
            return (Key)m_translateKey[keyCode];
        }

        switch (keyCode) {
            case NSF1FunctionKey:  return Key_F1;
            case NSF2FunctionKey:  return Key_F2;
            case NSF3FunctionKey:  return Key_F3;
            case NSF4FunctionKey:  return Key_F4;
            case NSF5FunctionKey:  return Key_F5;
            case NSF6FunctionKey:  return Key_F6;
            case NSF7FunctionKey:  return Key_F7;
            case NSF8FunctionKey:  return Key_F8;
            case NSF9FunctionKey:  return Key_F9;
            case NSF10FunctionKey: return Key_F10;
            case NSF11FunctionKey: return Key_F11;
            case NSF12FunctionKey: return Key_F12;

            case NSLeftArrowFunctionKey:   return Key_Left;
            case NSRightArrowFunctionKey:  return Key_Right;
            case NSUpArrowFunctionKey:     return Key_Up;
            case NSDownArrowFunctionKey:   return Key_Down;

            case NSPageUpFunctionKey:      return Key_PageUp;
            case NSPageDownFunctionKey:    return Key_PageDown;
            case NSHomeFunctionKey:        return Key_Home;
            case NSEndFunctionKey:         return Key_End;

            case NSPrintScreenFunctionKey: return Key_Print;
        }

        return Key_None;
    }
    
    //======================================================================================================================
    uint8_t InputSystemMacos::TranslateModifiers( int flags ) {
        return 0
            | ((0 != (flags & NX_DEVICELSHIFTKEYMASK ) ) ? KeyModifier_LeftShift  : 0)
            | ((0 != (flags & NX_DEVICERSHIFTKEYMASK ) ) ? KeyModifier_RightShift : 0)
            | ((0 != (flags & NX_DEVICELALTKEYMASK ) )   ? KeyModifier_LeftAlt    : 0)
            | ((0 != (flags & NX_DEVICERALTKEYMASK ) )   ? KeyModifier_RightAlt   : 0)
            | ((0 != (flags & NX_DEVICELCTLKEYMASK ) )   ? KeyModifier_LeftCtrl   : 0)
            | ((0 != (flags & NX_DEVICERCTLKEYMASK ) )   ? KeyModifier_RightCtrl  : 0)
            | ((0 != (flags & NX_DEVICELCMDKEYMASK) )    ? KeyModifier_LeftMeta   : 0)
            | ((0 != (flags & NX_DEVICERCMDKEYMASK) )    ? KeyModifier_RightMeta  : 0)
            ;
    }
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_INPUTSYSTEM_MACOS_H__
#define __XENGINE_INPUTSYSTEM_MACOS_H__

#include "xe/input/InputSystem.h"
#import <Cocoa/Cocoa.h>

namespace xe {
    
    class InputSystemMacos : public InputSystem {
    public:
        InputSystemMacos();
        
        ~InputSystemMacos();
        
        void Initialise();
        
        void InitialiseTranslateKey();
        
        void KeyEvent( NSEvent * ev );
        
        
    protected:
        
        Key HandleKeyEvent( NSEvent* event, uint8_t* specialKeys, uint8_t* _pressedChar );
        
        uint8_t TranslateModifiers( int flags );
        
    protected:
        int32_t     m_translateKey[256];
    };
}

#endif

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "misc/GameGlobals.h"

RTTI_CLASS_BEGIN( GameGlobals )
    RTTI_PROP(      OBJECT_REFPTR,           "camera",          m_camera )
    RTTI_PROP(      OBJECT_REFPTR,           "playfield",       m_playfield )
RTTI_CLASS_END( GameGlobals )

//======================================================================================================================
GameGlobals::GameGlobals() {
    
}

//======================================================================================================================
GameGlobals::~GameGlobals() {
    
}

//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLTYPES_H__
#define __COLTYPES_H__

namespace col {
    static const uint32_t GROUP_PLAYER = 1;
    static const uint32_t GROUP_ENEMY = 2;
    
    static const uint32_t GROUP_GAMEPLAY_OBJECTS = GROUP_PLAYER | GROUP_ENEMY;
    
    static const uint32_t GROUP_PLAYER_BULLET = 4;
    static const uint32_t GROUP_ENEMY_BULLET = 8;
}

#endif

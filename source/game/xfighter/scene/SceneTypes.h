//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SCENETYPES_H__
#define __SCENETYPES_H__

namespace scene {

    enum EVENT {
        EVENT_NULL = 0,
        EVENT_INPUT_ENABLE,
        EVENT_INPUT_DISABLE,
        EVENT_SPAWN_ENEMY,
        EVENT_SPAWN_PLAYER,
        EVENT_WAIT,
        EVENT_COUNT,
        EVENT_FORCE32 = 0xffffffff
    };

    struct Event {
        EVENT           m_type;
        uint32_t        m_time;
    };

    struct SpawnEnemy : public Event {
        uint32_t        m_enemyId;
        i3d::Vector2    m_pos;
    };

    struct SpawnPlayer : public Event {
        i3d::Vector2    m_pos;
    };
}

#endif

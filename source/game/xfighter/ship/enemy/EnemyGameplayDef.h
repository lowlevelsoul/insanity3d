//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENEMYGAMEPLAYDEF_H__
#define __ENEMYGAMEPLAYDEF_H__

#include "ship/ShipDef.h"

class EnemyGameplayDef : ShipComponentDef {
public:
    RTTI_CLASS_DECLARE( EnemyGameplayDef, ShipComponentDef )
    
    EnemyGameplayDef();
    
    virtual ~EnemyGameplayDef();
    
    virtual const char * GetTypeName();
    
public:
    float       m_energy;
};

#endif

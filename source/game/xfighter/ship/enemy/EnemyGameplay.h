//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENEMYGAMEPLAY_H__
#define __ENEMYGAMEPLAY_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class ColShape;
class Collider;
class Projectile;
struct OverlapResult;

class EnemyGameplay : ShipComponent {
public:
    RTTI_CLASS_DECLARE( EnemyGameplay, ShipComponent )
    
    static void HitCallback( const OverlapResult & res, Collider * thisObj, Collider * otherObj, void * context );
    
    EnemyGameplay();
    
    virtual ~EnemyGameplay();
    
    virtual void Construct( ShipComponentDef * def ) override;
    
    virtual void Think( float timeStep ) override;
    
    void HandleBullet( Projectile * projectile );
    
public:
    float       m_energy;
    float       m_damageThisFrame;
};

#endif
